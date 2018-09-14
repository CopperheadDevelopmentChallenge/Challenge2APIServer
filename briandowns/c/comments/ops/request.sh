#!/bin/sh

curl -v --user bdowns:one4all -XPOST \
 http://localhost:8080/1.0/relationships \
 -H 'Content-Type: application/json' \
 -d '{
   "correlation_id": "468691FF-D57F-4376-8468-C3BEA8977DC7",
   "email": "brian.downs@emailage.com"
}' | jq

exit 0
