(use '[ring.adapter.jetty :only (run-jetty)])

(defn app
  [{:key [uri query-string]}]
  {:body (format "You requested %s with query %s" uri query-string)})

(def server (run-jetty #'app {:port 8080 :join? false}))
 
