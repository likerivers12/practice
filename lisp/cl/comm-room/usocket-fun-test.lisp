(require 'usocket)

(defpackage :usocket-fun-test
  (:use :cl :usocket))

(in-package :usocket-fun-test)

(defparameter *addr* #(127 0 0 1))
(defparameter *port* 8010)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 1:Server 
(defparameter *server-sock* (socket-listen *addr* *port*
						   :reuse-address t
						   :backlog 1
						   :element-type 'character))

*server-sock*
(socket *server-sock*)

(get-local-name *server-sock*)
;(get-local-name-address *server-sock*)
(get-local-port *server-sock*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 2:Server 
(defparameter *server-con* (usocket:socket-accept *server-sock*))

*server-con*

(socket-stream *server-con*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 3. client
(defparameter *client-con* (socket-connect *addr* *port*))

*client-con*

(socket-stream *client-con*)

(get-local-name *client-con*)
(get-local-port *client-con*)

;(get-peer-name *client-con*)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 4. check
(get-peer-name *client-con*)
(get-peer-port *client-con*)

(get-peer-name *server-con*)
(get-peer-port *server-con*)

(get-local-name *server-con*)
(get-local-port *server-con*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 5. Server Socket Stream
(defparameter *server-stream* (socket-stream *server-con*))
*server-stream*

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 6. Client Socket Stream
(defparameter *client-stream* (socket-stream *client-con*))
*client-stream*

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 7. Send a Message Client -> Server
(format *client-stream* "Message From Client~%")
(force-output *client-stream*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 8. Receive a Message from Client
(format t "~A~%" (read-line *server-stream* nil nil))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 9. Send a Message : Server -> Client
(format *server-stream* "Message From Server~%")
(force-output *server-stream*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 10. Receive a Message from Server
(format t "~A~%" (read-line *client-stream* nil nil))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 11. close sockets
(socket-close *client-con*)

(socket-close *server-con*)
(socket-close *server-sock*)

