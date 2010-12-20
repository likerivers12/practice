(require 'sb-bsd-sockets)

;; Most of the following is cribbed from the trivial-sockets package
;; (http://www.cliki.net/trivial-sockets).  I've ripped it out as it's
;; not clear that the return values will remain the same in future
;; versions and I need to get at the underlying file descriptors for
;; serve-event.
(defun resolve-hostname (name)
  (cond
    ((eql name :any)  #(0 0 0 0))
    ((typep name '(vector * 4)) name)
    (t (car (sb-bsd-sockets:host-ent-addresses
         (sb-bsd-sockets:get-host-by-name name))))))
 
(defun open-server (&key (host :any) (port 0)
		    (reuse-address t)
		    (backlog 1)
		    (protocol :tcp))
  "Returns a server socket"
  (let ((sock (make-instance 'sb-bsd-sockets:inet-socket
			     :type :stream
			     :protocol protocol)))
    (when reuse-address
      (setf (sb-bsd-sockets:sockopt-reuse-address sock) t))
    (sb-bsd-sockets:socket-bind  sock (resolve-hostname host) port)
    (sb-bsd-sockets:socket-listen sock backlog)
    sock))
 
(defmacro with-server ((name arguments) &body forms)
  `(let (,name)
     (unwind-protect
	  (progn
	    (setf ,name (open-server ,@arguments))
	    ,@forms)
       (when ,name (sb-bsd-sockets:socket-close ,name)))))
 
;; ;; End trivial-sockets ;; ;;
 
;; Server implementation
 
(defconstant +buflen+ 16)      ; Short for overrun testing
(defstruct server-session
  sock ;; Socket returned by accept
  fd ;; Raw file handle
  stream ;; Lisp stream
  buffer ;; Pre-allocated incoming buffer
  handler ;; serve-event handler
  )
 
(defun data-received-handler (session)
  "Reads all pending characters on a socket into the session buffer"
  (format t "Got incoming data event ... ~%")
  (let ((buffer (server-session-buffer session))
        (sock (server-session-sock session)))
    (do ((fin nil))
	(fin t)
      (setf (fill-pointer buffer) +buflen+)
      (multiple-value-bind (buf len raddr)
          (sb-bsd-sockets:socket-receive sock buffer nil)
        (declare (ignore raddr))
        (if (null buf)
            (setf fin t)
            (setf (fill-pointer buffer) len)))
      (cond ((= (length buffer) 0)
	     (sb-bsd-sockets:socket-close sock)
             (sb-sys:remove-fd-handler (server-session-handler session))
             (setf fin t))
            (fin (format t "Got NIL, returning~%"))
            (t
             (format t "  Read ~a bytes: ~a~%" (length buffer) buffer))))))
 
(defun accept-handler(socket)
  (format t "I've got a new connection on fd ~a~%" socket)
  (let* ((conn (sb-bsd-sockets:socket-accept socket))
         (fd (sb-bsd-sockets:socket-file-descriptor conn))
         (session (make-server-session
		   :sock conn
		   :fd fd
		   :stream (sb-bsd-sockets:socket-make-stream 
			    conn :input t :output t
			    :element-type 'character
			    :buffering :none)
		   :buffer (make-array +buflen+
				       :element-type 'character
				       :adjustable nil
				       :fill-pointer t)))
         (handler (sb-sys:add-fd-handler
		   fd :input
		   #'(lambda (fd) (declare (ignore fd))
			     (data-received-handler session)))))
    (format t "New socket is ~a~%" conn)(force-output)
    (setf (sb-bsd-sockets:non-blocking-mode conn) t)
    (setf (server-session-handler session) handler)))
 
 
(defun start-server ()
  (with-server (socket (:port 8000 :reuse-address t))
    (sb-sys:with-fd-handler ((sb-bsd-sockets:socket-file-descriptor socket)
                             :input #'(lambda (fd) (declare (ignore fd))
					      (accept-handler socket)))
      (loop ;; FIXME: End condition
	 (format t "Entering serve-all-events...~%")(force-output)
	 (sb-sys:serve-all-events)
	 (format t "Events served~%")))))
