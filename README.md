# HPSC Lab 3
2019-09-13

Click to [accept this assignment](https://classroom.github.com/a/pla4omKL).

The goals for this lab are:
* Practice using the tools needed for homework 1 (e.g. git, OpenMP)
* Explore code techniques when encountering a variable amount of work per index, aiming to understand optimization tradeoffs.
* Produce evidence of critical thinking.  Please push the requested files for the first two sections; the third section is encouraged but flexible.

WEEKLY WORDS OF CAUTION:
It is better to take the time to check how large your size parameters can get *before* overallocating.

-----

Aim to move on from this section after at most 30 minutes.

This lab starts from the code in mandelbrot.c.  The **Mandelbrot set** is defined to be the set of complex numbers $$a+ib$$ for which the iterated function $$f_{n+1}(c) = f_n(a+ib) + a + ib$$ does **not** diverge.  If you'd like to delve deeper there is bountiful literature on the Mandelbrot set, including some important open conjectures.  For this lab we'll just jump in with a few baseline assumptions.

It turns out that the set has a single fractal edge.  We can approximate the shape by iterating candidate points some large number of times.  That is, any candidate for which we can detect divergence of the iterated functon within these iterations is definitively **not** in the Mandelbrot set (although not detecting divergence is not conclusive, by itself).  Implicit from this description, such approximations of the set involve (at least) two parameters, which we'll call:
$$d$$ --- density parameter; the frequency of candidate points on a unit interval
$$m$$ --- max iteration parameter, when we give up on detecting divergence

Take a look at the ```mandelbrot_bruteforce.c```, including ```shared.h```.  This code leverages a few additional properties:
* An easy-to-use test for divergence is whether the complex radius is greater than $$2$$.  That is, for a current iteration $$f_n(a+ib) = a_n+ib_n$$, divergence is guarenteed if $$a_n^2 + b_n^2 > 2^2 = 4$$.
* The set is symmetric over the real axis, so we can cut our time in half by evaluating only candidates with non-negative imaginary parts.

Compile and run this code, redirecting output to a file:
```gcc -O3 mandelbrot_bruteforce.c -o mandelbrot_bruteforce```
```./mandelbrot_bruteforce > outfile```

The first row of output gives the $$(a_min,a_max,b_min,b_max)$$ domain of candidates.  The remaining rows each represent a successive value of $$a$$, with spaced entries corresponding to successive values of $$b$$, having value equal to whether they were within the divergence threshold after the function iterations completed.  We provide a python script (requiring the matplotlib module) to visualize this, where the horizontal axis is the real part, the vertical axis is the imaginary part, and the color designates whether the point "survived" all of the iterations:
```python visualize.py outfile```
[For those running on a remote machine without graphics, like CSEL, ```visualize.py``` accepts an optional ```-o [path]``` flag to output the image to a file, which you can then open in a browser or copy back to your local machine.  Of course, the downside is that you can't interactively pan/zoom.  Copying the ```outfile``` to your local machine is another option, although it can get large for high ```-d```.]

The fractal nature should already be noticable! (with the defaults ```-d 100 -m 1000```)  Play with adjusting ```-d```; you should (once again!) be able to see the effects of cache sizes on the throughput rate (here measured in maps/tick).  WARNING: memory usage scales quadratically with ```-d```!  For reference ```-d 8000``` uses just about 8 GB of RAM and creates a 4.3 GB output file.

You should have enough practice by now (or review previous labs!) to make a plot of throughput (maps/tick) as a function of ```-d``` for a wide range of ```-d``` (stay in volitile memory).  You'll need to perform your own repetitions to assess noise.  Name such a plot ```part1_plot.png``` (or .jpg) and commit and push back to github.  Also look at the visualizations for a few orders of magnitude of ```-d``` and make a paragraph of some qualitative observations, pushed back to github as ```part1_notes.md```.

-----

Aim to leave no less than the last 10 minutes of lab to at least read through and think about the third section.

Look inside the ```baseline``` version and compare it to the previous ```bruteforce``` version.  Most importantly, the baseline version changes the loop order such that the function iteration is in the innermost loop.  This not only permits more memory-access efficient, but by checking against the same divergence threshold we can avoid processing further as soon as the point exceeds the threshold.  Of course, one additional cost to this approach is having to pay the processing costs to check the divergence threshold.

At the cost of 50% more memory and some miniscule compute, the visualization will now show at what fraction of the maximum iterations the candidate point escaped the divergence threshold.  You may need to zoom in to notice the different pixels.  Another popular visualization is to color each candidate based on the cumulative fraction of escaped candidates which escaped sooner.

There are a number of other "obvious" algorithmic improvements.  We easily can ascertain upfront large areas of candidate that are provably in the Mandelbrot set.  We can remove such points from our calculations knowing that each would have survived all of the function iterations, which saves a huge amount of work.
* Let $$s = (a-1/4)^2+b^2$$.  Candidate points that satisfy $$s(s+a-1/4) \le b^2/4$$ are in the largest blob in the Mandelbrot set.
* Candidate points that satisfy $$(a+1)^2+b^2 \le 1/16$$ are in the second-largest blob in the Mandelbrot set.  These points have a periodicity of 2.
These two additions are implemented in ```mandelbrot_weeded.c```.  How many function iterations do each of these save for the same resolution and maximum number of iterations?

Try adding/toggling various optimizations covered in class and last lab: vectorization, multithreading with omp, etc.  You should find that vectorization fractionally improves ```bruteforce``` far more than it improves  ```weeded``` (which it shouldn't improve really at all).  Why?

With multithreading, in class on Wednesday we discussed two intuitive explicit ways to assign work to each thread: cyclicly or in blocks.  Try both of these on the outer loop of ```baseline```.  Why might the block method actually perform worse in this case?

**What steps might you take to be able to better use these parallelism tools?**  Push to github a couple paragraphs describing your findings and thoughts as ```part2_notes.md```.

-----

In general having a variable amount of work per index makes it trickier to leverage vectorization and multithreading.  Hopefully in the last section you've thought about other ways to structure the loops to better leverage these tools.  Try implementing some of these ideas.

As another option for an algorithmic tweak, note that any periodicity indicates inclusion in the Mandelbrot set, since such sequences can never diverge.  However, implementing periodicity checking on the fly requires balancing against the additional computation costs, and effects from roundoff errors can be become problematic.  Try it if you like.

How do different combinations of algorithmic changes impact the best way to structure the loops for *higher* resolution or *larger* maximum number of iterations?   Can you think of additional data techniques that become worthwhile, which may have added too much overhead previously?

Push any alternative versions of code as ```mandelbrot_[name].c```.  Push a representative subset of thoughts and findings (including any different compile options) to ```part3_notes.md```.  Push a favorite visualization (perhaps with different A_MIN,A_MAX,B_MIN,B_MAX values) to ```part3_vis.png``` (or .jpg).