# HPR-BLAS
High-Performance Reproducible BLAS using posit arithmetic

# Introduction

Round-off makes floating point addition nonassociative, and different orders of summation often produce different results.
On a parallel machine, the order of summation will vary from run to run, or even subroutine-call to subroutine-call, depending on scheduling of available resources, so results will different from run to run.
HPR-BLAS offers high-performance reproducible BLAS independent of concurrency levels of the underlying hardware.

The obvious benefit for reproducibility is that debugging in real-time, multi-core systems is now feasible. The lack of IEEE Floating Point associativity has made certain bugs elusive in real-time, multi-core systems. HPR-BLAS is associative, so any roundoff-related errors caused by spurious events can be productively debugged.

HPR-BLAS enables new applications is very demanding environments. For example, in fracture mechanics,
a shutgun approach is used to generate many random simulations looking for rare events. When one occurs,
an accurate resimulation is started while computing additional information. Reproducibility is essential
to yield correct results.

HPR-BLAS is using posit arithmetic. Posit arithmetic is a tapered floating point system with perfect symmetry around 0 and +-infinity. Posits allow custom tailoring of accuracy and dynamic range to the demands of the application. This is particularly important for Deep Learning applications where each layer in the network has unique requirements for accuracy and dynamic range. 


