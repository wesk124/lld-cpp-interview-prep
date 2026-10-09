# OOP Interview Foundations Quiz

Answer before opening the explanations.

1. ParkingLot stores const PricingPolicy&. What lifetime relationship does this express?

   A. The lot owns and deletes the policy  
   B. The caller keeps the borrowed policy alive while the lot uses it  
   C. The policy is copied into every ticket  
   D. The policy must be global

2. Why keep a numeric spot ID in a ticket rather than a pointer to a vector element?

   A. IDs provide object identity independent of container storage  
   B. Integers automatically synchronize threads  
   C. IDs replace all validation  
   D. Pointers can never identify objects

3. If concurrency is added, what should a lock protect?

   A. A class name  
   B. A stated invariant across related state  
   C. Only one map access at a time  
   D. Every function regardless of shared state

4. When is Strategy useful?

   A. Whenever there are two classes  
   B. When a behavior can vary independently of its coordinating workflow  
   C. Only for numeric algorithms  
   D. To avoid implementing a workflow

5. Why does Node have a virtual destructor?

   A. To sort children  
   B. To safely destroy File/Directory through unique_ptr<Node>  
   C. To make every node globally accessible  
   D. To forbid all copying

6. State three parking-lot invariants.
7. Explain why ParkingSpot values work well inside a vector owned by the lot.
8. Which state belongs in the same park/checkout critical section if threads are added?
9. How can you test the 60/61-minute fee boundary without sleeping?
10. How can flat pricing replace hourly pricing, and which class remains unchanged?

[Answer key](solutions.md)
