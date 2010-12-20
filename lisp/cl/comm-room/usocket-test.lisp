(require 'usocket)

;; ok!
(defun run-client (port)
  (with-open-stream (s (usocket:socket-stream (usocket:socket-connect "127.0.0.1" port)))
    (format s "Socket test from client~%")
    (force-output s)
    (sleep 3)
    (let ((line "AAA"))
      (format t "Recv from Server:[~A]~%" line))
    (sleep 5)))

;(run-client 8001)

