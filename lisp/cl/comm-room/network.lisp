;;; http://www.lisphacker.com/projects/clxs/repos/clxs/network.lisp

;;;
;;; network.lisp
;;;
;;; Network Interface
;;;
;;; Copyright (C) 2005, Alastair Bridgewater
;;;

(in-package :clxs)

;;; FIXME: Far too sparse, needs documentation.
;;; FIXME: Disconnect callbacks?
;;; FIXME: Output path?
;;; FIXME: Closing a connection from this side?

(defconstant +request-buffer-size+ 16384
  "Minimum size (in octets) allowed for the upper limit on X request sizes")

(defclass network-server ()
  (connect-callback
   state
   host
   port
   socket))

(defclass network-client ()
  (socket
   packet-size
   packet-data
   packet-received-callback))

(defun make-network-server (host port &key mp-style connect-callback)
  (declare (type (unsigned-byte 16) port))
  (declare (type (or function null) connect-callback))
  (unless (eq mp-style :serve-event) (error "MP-STYLE not :SERVE-EVENT (required)"))
  (let ((retval (make-instance 'network-server)))
    ;; FIXME: There's probably a better way to do this.
    (setf (slot-value retval 'connect-callback) connect-callback)
    (setf (slot-value retval 'state) :stopped)
    (setf (slot-value retval 'host) host)
    (setf (slot-value retval 'port) port)
    (setf (slot-value retval 'socket) nil)
    retval))

(defgeneric network-server-connect-callback (network-server))
(defgeneric (setf network-server-connect-callback) (callback network-server))
(defgeneric network-server-invoke-connect-callback (network-server))

(defmethod network-server-connect-callback ((network-server network-server))
  (slot-value network-server 'connect-callback))

(defmethod (setf network-server-connect-callback)
    (callback (network-server network-server))
  (when (and (null callback)
	     (eq (slot-value network-server 'state) :running))
    (error "Can't set NIL callback while server running"))
  (setf (slot-value network-server 'connect-callback) callback))

(defmethod network-server-invoke-connect-callback
    ((network-server network-server))
  (funcall (network-server-connect-callback network-server) network-server))

(defgeneric network-server-start (network-server))
(defgeneric network-server-stop (network-server))
(defgeneric network-server-status (network-server))

(defmethod network-server-start ((network-server network-server))
  (with-slots (host port socket state connect-callback) network-server
    (when (eq state :running) (error "server already running"))
    (unless connect-callback (error "no connect callback"))
    (setf state :running)
    (setf socket (make-instance 'sb-bsd-sockets:inet-socket
				:type :stream :protocol :tcp))
    (setf (sb-bsd-sockets:sockopt-reuse-address socket) t)
    (sb-bsd-sockets:socket-bind socket host port)
    (sb-bsd-sockets:socket-listen socket 5)
    (sb-sys:add-fd-handler
     (sb-bsd-sockets:socket-file-descriptor socket)
     :input (lambda (x) (declare (ignorable x))
		    (network-server-invoke-connect-callback network-server))))
  t)

(defmethod network-server-stop ((network-server network-server))
  (with-slots (socket state) network-server
    (unless (eq state :running) (error "server not running"))
    (setf state :stopped)
    (sb-sys:invalidate-descriptor
     (sb-bsd-sockets:socket-file-descriptor socket))
    (sb-bsd-sockets:socket-close socket)
    (setf socket nil))
  t)

(defmethod network-server-status ((network-server network-server))
  (slot-value network-server 'state))

(defgeneric network-server-accept (network-server))

(defmethod network-server-accept ((network-server network-server))
  (let ((network-client (make-instance 'network-client))
	(socket (sb-bsd-sockets:socket-accept
		 (slot-value network-server 'socket))))
    (setf (slot-value network-client 'socket) socket)
    (setf (slot-value network-client 'packet-data)
	  (make-array +request-buffer-size+
		      :element-type '(unsigned-byte 8)
		      :fill-pointer 0))
    (setf (slot-value network-client 'packet-size) 0)
    (setf (slot-value network-client 'packet-received-callback) nil)
    (sb-sys:add-fd-handler
     (sb-bsd-sockets:socket-file-descriptor socket)
     :input (lambda (x) (declare (ignorable x))
		    (network-client-receive-data network-client)))
    network-client))

(defgeneric network-client-disconnect (network-client))

(defmethod network-client-disconnect ((network-client network-client))
  (with-slots (socket) network-client
    (sb-sys:invalidate-descriptor
     (sb-bsd-sockets:socket-file-descriptor socket))
    (sb-bsd-sockets:socket-close socket)))

(defun network-client-receive-data (network-client)
  (declare (type network-client network-client))
  ;; Okay, how's this for amusing? Looks like we need to use the posix
  ;; interfaces to do the actual reads, since standard CL doesn't
  ;; provide the semantics we want.
  ;; Core logic lifted from fd-stream.lisp in the SBCL sources.
  (with-slots (packet-data packet-size socket) network-client
    (multiple-value-bind (count errno)
	(sb-unix:unix-read (sb-bsd-sockets:socket-file-descriptor socket)
			   (sb-sys:int-sap (+ (sb-sys:sap-int
					       (sb-sys:vector-sap
						(sb-impl::%array-data-vector
						 packet-data)))
					      (fill-pointer packet-data)))
			   (- packet-size (length packet-data)))
      (cond ((null count)
	     (if (eql errno sb-unix:ewouldblock)
		 (progn)
		 ;; Will be sb-unix:econnreset if the remote disconnects.
		 (error "couldn't read from ~S: ~S" network-client errno)))
	    ((zerop count)
	     ;; Two options: EOF or we just asked for zero bytes.
	     ;; Asking for zero bytes is a bug, plain and simple. It could
	     ;; happen, but it needs to be detected at the packet-received
	     ;; callback level. We shall concern ourselves with EOF now.
	     (network-client-disconnect network-client)
	     ;; FIXME: A disconnect callback, either here or in the disconnect
	     ;; handler above, is definately in order.
	     )
	    (t
	     (incf (fill-pointer packet-data) count))))
    (when (= (fill-pointer packet-data) packet-size)
      (network-client-invoke-packet-received-callback network-client packet-data))))

(defgeneric network-client-packet-size (network-client))
(defgeneric (setf network-client-packet-size) (new-size network-client))

(defmethod network-client-packet-size ((network-client network-client))
  (slot-value network-client 'packet-size))

(defmethod (setf network-client-packet-size)
    (new-size (network-client network-client))
  (setf (slot-value network-client 'packet-size) new-size))

(defgeneric network-client-packet-received-callback (network-client))
(defgeneric (setf network-client-packet-received-callback) (callback network-client))
(defgeneric network-client-invoke-packet-received-callback (network-client packet-data))

(defmethod network-client-packet-received-callback
    ((network-client network-client))
  (slot-value network-client 'packet-received-callback))

(defmethod (setf network-client-packet-received-callback)
    (callback (network-client network-client))
  (setf (slot-value network-client 'packet-received-callback) callback))

(defmethod network-client-invoke-packet-received-callback
    ((network-client network-client) packet-data)
  (funcall (network-client-packet-received-callback network-client)
	   network-client packet-data))

(defgeneric network-client-packet-complete (network-client))

(defmethod network-client-packet-complete ((network-client network-client))
  (setf (fill-pointer (slot-value network-client 'packet-data)) 0)
  nil)

;; FIXME: Cheap hack interface, should be formalized.
(defun send-packet (network-client packet)
  (sb-unix:unix-write
   (sb-bsd-sockets:socket-file-descriptor
    (slot-value network-client 'socket))
   (sb-sys:vector-sap packet)
   0 (length packet)))

;;; EOF
