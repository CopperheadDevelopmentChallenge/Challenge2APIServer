# Challenge 2 API Server

## Deadline: 1537401599 MST

## The Challenge

We are building an api server that will take the provided `data.json` file in the root of the repo load it into memory and serve that data via REST endpoints.

Upon start the service will load the json file and store it in memory. then be able to update delete, or add to it.

## The requirements

- Must implement all endpoints mapped out below
- Must have at least 2 cli flags
  - a flag to set the port
  - a flag to set the host

### The endpoints

- /comments: 
  - Method: GET 
  - Status Code Success: 200 
  - Desc: get all the comments
  - Payload:

```json
[
  {
    "id": 32,
    "name": "dolorem architecto ut pariatur quae qui suscipit",
    "email": "Maria@laurel.name",
    "body": "nihil ea itaque libero illo\nofficiis quo quo dicta inventore consequatur voluptas voluptatem\ncorporis sed necessitatibus velit tempore\nrerum velit et temporibus"
  }
]
```
- /comments?size=10&from=1
  - Method: GET
  - Status Code Success: 200
  - Query String:
    - size: how many comments to return
    - from: offset of comments to return
  - Desc: Only get a selected amount of comments starting with the from offset
    - from: 1 == array[0]
  - Payload: 

```json
[  
  {
    "id": 32,
    "name": "dolorem architecto ut pariatur quae qui suscipit",
    "email": "Maria@laurel.name",
    "body": "nihil ea itaque libero illo\nofficiis quo quo dicta inventore consequatur voluptas voluptatem\ncorporis sed necessitatibus velit tempore\nrerum velit et temporibus"
  }
]
```

- /comments/{:id}
  - Method: GET
  - Status Code Success: 200
  - Params:
    - id: id to specific comment
  - Desc: Get a single comment by ID
  - Payload:

```json
  {
    "id": 32,
    "name": "dolorem architecto ut pariatur quae qui suscipit",
    "email": "Maria@laurel.name",
    "body": "nihil ea itaque libero illo\nofficiis quo quo dicta inventore consequatur voluptas voluptatem\ncorporis sed necessitatibus velit tempore\nrerum velit et temporibus"
  }
```

- /comments
  - Method: POST
  - Status Code Success: 201
  - Desc: based on the
  - Input: 

```json
  {
    "name": "dolorem architecto ut pariatur quae qui suscipit",
    "email": "Maria@laurel.name",
    "body": "nihil ea itaque libero illo\nofficiis quo quo dicta inventore consequatur voluptas voluptatem\ncorporis sed necessitatibus velit tempore\nrerum velit et temporibus"
  }
```

  - Payload:

```json
  {
    "id": 33,
    "name": "dolorem architecto ut pariatur quae qui suscipit",
    "email": "Maria@laurel.name",
    "body": "nihil ea itaque libero illo\nofficiis quo quo dicta inventore consequatur voluptas voluptatem\ncorporis sed necessitatibus velit tempore\nrerum velit et temporibus"
  }
```

- /comments/{:id}
  - Method: PUT
  - Status Code Success: 202
  - Desc: based on the  
  - Input:

```json
  {
    "id": 33,
    "name": "Changed",
    "email": "Maria@laurel.name",
    "body": "nihil ea itaque libero illo\nofficiis quo quo dicta inventore consequatur voluptas voluptatem\ncorporis sed necessitatibus velit tempore\nrerum velit et temporibus"
  }
```

  - Payload:

```json
  {
    "id": 33,
    "name": "Changed",
    "email": "Maria@laurel.name",
    "body": "nihil ea itaque libero illo\nofficiis quo quo dicta inventore consequatur voluptas voluptatem\ncorporis sed necessitatibus velit tempore\nrerum velit et temporibus"
  }
```

- /comments/{:id}
  - Method: DELETE
  - Status Code Success: 202
  - Desc: based on the  
  - Payload:

```json
  {
    "id": 33,
    "name": "Changed",
    "email": "Maria@laurel.name",
    "body": "nihil ea itaque libero illo\nofficiis quo quo dicta inventore consequatur voluptas voluptatem\ncorporis sed necessitatibus velit tempore\nrerum velit et temporibus"
  }
```

## How to add your project to this challenge

- You can fork the repo and and do a pull request to this (upstream repo)
  - https://gist.github.com/Chaser324/ce0505fbed06b947d962
  - Please do not merge yourself
- Please use the following directory structure:
  - /gitroot/githubusernme/languagename/projectroot
    - git root: is the root dir of the cloned git repo
    - github username: so we know who submitted it
    - language name: name of your project language
      - I added this so you may try the challenge in multiple languages if you please
      - please keep the name simmple without any special characters
        - C# should be csharp and not c# or c-sharp
    - project root: is where your project lives
      - example: with rust i run `cargo new project --bin` the `src/ Cargo.toml Cargo.lock` should live in project root

