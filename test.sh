#!/bin/bash

# Number of simultaneous requests
NUM_REQUESTS=140

# URL to send requests to
URL="http://localhost:8000/index.html"
URL2="http://localhost:8001/index.html"
URL3="http://localhost:8002/index.html"

X=0
# Function to send HTTP request and print response
send_request() {
    local response
    # Capture the response and store it in a variable
    response=$(curl -s "$URL")
    response2=$(curl -s "$URL2")
    response3=$(curl -s "$URL3")
    
    echo "$X"
    # Print the response
    # echo "Response1: $response"
    #  echo "Response2: $response2"
    #   echo "Response3: $response3"
}

# Send requests in parallel
for ((i=1; i<=$NUM_REQUESTS; i++)); do
    ((X++))
    # Start a background process to send the request
    send_request &
done

# Wait for all requests to finish
wait

echo "All requests sent and responses printed"
