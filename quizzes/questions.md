# C++ LLD Foundations Quiz

Try answering each question before opening the explanations.

## Multiple choice

1. A `ParkingLot` exclusively owns a replaceable pricing policy. Which representation best communicates that ownership?

   A. Raw pointer  
   B. `std::unique_ptr<PricingPolicy>`  
   C. `std::shared_ptr<PricingPolicy>`  
   D. Global variable

2. Which design best avoids dangling references when tickets outlive internal container reallocations?

   A. Store a pointer to a `ParkingSpot` in every ticket  
   B. Store the spot's stable ID in the ticket  
   C. Reserve a large vector and assume it never grows  
   D. Make every spot global

3. What should a mutex protect?

   A. A class name  
   B. An individual function  
   C. A stated shared-state invariant  
   D. Every `const` operation

4. When is Strategy justified in an interview design?

   A. Whenever two classes exist  
   B. When an expected policy must vary independently of the core workflow  
   C. Only when the interviewer names the pattern  
   D. To avoid writing tests

5. Why should a polymorphic base such as `PricingPolicy` have a virtual destructor?

   A. It makes construction faster  
   B. It permits safe destruction through a base pointer  
   C. It prevents copying automatically  
   D. It makes all methods virtual

## Short answer

6. State three invariants for a parking-lot design.

7. Explain why value semantics may be preferable to dynamic allocation for `ParkingSpot`.

8. Name the shared state involved in an atomic checkout operation.

9. How would you test fee calculation without depending on wall-clock time?

10. An interviewer adds weekend pricing. Which code should change, and which code should remain unchanged?
