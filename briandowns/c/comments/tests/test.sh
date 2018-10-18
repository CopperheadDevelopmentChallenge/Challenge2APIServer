#!/bin/sh

BASE_URL="http://127.0.0.1:8080"

CONTAINER_ID=$(docker run -d -p 8080:8080 -t bdowns-copperhead)
if [ $? != 0 ]; then
    exit 127
fi

echo "Running container id: "${CONTAINER_ID}

echo "Running /comments test..."
TEST_URL="${BASE_URL}/comments" 
curl --silent -XGET ${TEST_URL} >> /dev/null
if [ $? != 0 ]; then 
    echo Testing - ${TEST_URL} [FAILED] 
else
    echo Testing - ${TEST_URL} [OK]
fi

docker ps

echo "Running /comments/{id} test..."
for i in $(seq 1 100); do 
    TEST_URL="${BASE_URL}/comments/${i}" 
    curl --silent -XGET ${TEST_URL} >> /dev/null
    if [ $? != 0 ]; then 
        echo Testing - ${TEST_URL} [FAILED] 
    else
        echo Testing - ${TEST_URL} [OK]
    fi
done

docker kill ${CONTAINER_ID}
docker rm ${CONTAINER_ID}

exit 0
