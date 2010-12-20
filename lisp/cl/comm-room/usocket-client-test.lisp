(require 'usocket)

(defpackage :usocket-test
  (:use :cl :usocket))

(in-package :usocket-test)

;; OK!
(defun run-client (port)
  (let* ((con (usocket:socket-connect "127.0.0.1" port)))
    (with-open-stream (s (usocket:socket-stream con))
      (format s "Socket test from client~%")
      (force-output s)
      (sleep 1)
      (let ((line (read-line s nil nil)))
	(format t "Recv from Server:[~A]~%" line)))
    (usocket:socket-close con)))

;(run-client 8001)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; OK!
;;  defun run-client (port)
;;   (with-open-stream (s (usocket:socket-stream (usocket:socket-connect "127.0.0.1" port)))
;;     (format s "Socket test from client~%")
;;     (force-output s)
;;     (sleep 1)
;;     (let ((line (read-line s nil nil)))
;;       (format t "Recv from Server:[~A]~%" line))
;; ))


