# Parking Lot Requirements

This solution follows the exercise contract from the matching [interview question](../../questions/01-parking-lot/README.md).

The implementation chooses the smallest compatible available spot, treats tickets as single-use stable records, rounds billing up to whole hours with a one-hour minimum, and protects all in-memory state with one mutex.
