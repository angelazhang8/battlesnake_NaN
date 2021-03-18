# battlesnake_NaN

The Replit code is found here: [https://replit.com/@focusmuststudy/battlesnakeNaN#code/move.cpp](https://replit.com/@focusmuststudy/battlesnakeNaN#code/move.cpp)

The Replit server is found here: [https://battlesnakeNaN.focusmuststudy.repl.co](https://battlesnakeNaN.focusmuststudy.repl.co)

### Todo for the week of March 15:
* Fix the segfault problem that pops up around 20% of the time: `Segmentation fault      (core dumped) ./main`
* Refine the basic algorithm, moving in a direction containing no obstacles, in preparation for the mini tournament

An example of JSON data from the POST request is:
```
{"board":{"food":[{"x":8,"y":2},{"x":10,"y":10},{"x":10,"y":4},{"x":6,"y":8},{"x":0,"y":0},{"x":2,"y":8},{"x":0,"y":6},{"x":4,"y":2}],"hazards":[],"height":11,"snakes":[{"body":[{"x":9,"y":10},{"x":9,"y":9},{"x":9,"y":8}],"head":{"x":9,"y":10},"health":91,"id":"gs_kTKcxB9w99BfGqD3rSbjtPhM","latency":"58","length":3,"name":"testsnake","shout":""},{"body":[{"x":1,"y":10},{"x":1,"y":9},{"x":1,"y":8}],"head":{"x":1,"y":10},"health":91,"id":"gs_XrdgvBhQGD34j86TRgFwP3D3","latency":"61","length":3,"name":"testsnake","shout":""},{"body":[{"x":5,"y":10},{"x":5,"y":9},{"x":5,"y":8},{"x":5,"y":7}],"head":{"x":5,"y":10},"health":95,"id":"gs_p4RGYfGGkCfjqBhYqVBRjQKM","latency":"120","length":4,"name":"testsnake","shout":""}],"width":11},"game":{"id":"e437218a-ddd4-486c-a18f-cb0af1ae05bc","ruleset":{"name":"standard","version":"v1.0.17"},"timeout":500},"turn":9,"you":{"body":[{"x":9,"y":10},{"x":9,"y":9},{"x":9,"y":8}],"head":{"x":9,"y":10},"health":91,"id":"gs_kTKcxB9w99BfGqD3rSbjtPhM","latency":"58","length":3,"name":"testsnake","shout":""}}
```
