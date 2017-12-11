



<p>In this exercise, we are optimizing and benchmarking our stencil implementation. We are only focusing on the 2D stencil for now.</p>
<h2><a href="#optimizations" aria-hidden="true" class="anchor" id="user-content-optimizations"><svg aria-hidden="true" class="octicon octicon-link" height="16" version="1.1" viewBox="0 0 16 16" width="16"><path fill-rule="evenodd" d="M4 9h1v1H4c-1.5 0-3-1.69-3-3.5S2.55 3 4 3h4c1.45 0 3 1.69 3 3.5 0 1.41-.91 2.72-2 3.25V8.59c.58-.45 1-1.27 1-2.09C10 5.22 8.98 4 8 4H4c-.98 0-2 1.22-2 2.5S3 9 4 9zm9-3h-1v1h1c1 0 2 1.22 2 2.5S13.98 12 13 12H9c-.98 0-2-1.22-2-2.5 0-.83.42-1.64 1-2.09V6.25c-1.09.53-2 1.84-2 3.25C6 11.31 7.55 13 9 13h4c1.45 0 3-1.69 3-3.5S14.5 6 13 6z"></path></svg></a>Optimizations</h2>
<p>We want to investigate several different optimizations:</p>
<ol>
<li>Use 2D subdivision of the grid instead of 1D subdivision.</li>
<li>Perform all send and receive operations without blocking.</li>
<li>Interleave the inner computation of each tile of the stencil with communicating the ghost cells.</li>
<li>[Bonus] One more optimization (of the parallel execution, not the sequential program) that you come up with on your own.</li>
</ol>
<p>For each of these optimizations, track your progress by using the benchmarking regimen outlined below.</p>
<p>Ascii art for the third optimization (showing a single tile and its surroundings):</p>
<pre><code>     ...                     ...
      |                       |
      |    2                  |
...-------------------------------...
      |    1          4       |
      |  |-----------------|  |
    2 |1 | 3. compute      |1 |2
      |  |    inner        |  |
      |4 |-----------------|4 |
      |    1          4       |
...-------------------------------...
      |    2                  |
      |                       |
     ...                     ...
</code></pre>
<p>In step 1, the outermost cells (the surrounding tiles' ghost cells) are sent out.
Then, in step 2, the own ghost cells are received <em>asynchronously</em>. Step 3, computing
the inner cells, is started immediately. Then, we block until the ghost cells have been received
(which likely already happened). In step 4, we calcualte the new ghost cells.</p>
<h2><a href="#benchmarking" aria-hidden="true" class="anchor" id="user-content-benchmarking"><svg aria-hidden="true" class="octicon octicon-link" height="16" version="1.1" viewBox="0 0 16 16" width="16"><path fill-rule="evenodd" d="M4 9h1v1H4c-1.5 0-3-1.69-3-3.5S2.55 3 4 3h4c1.45 0 3 1.69 3 3.5 0 1.41-.91 2.72-2 3.25V8.59c.58-.45 1-1.27 1-2.09C10 5.22 8.98 4 8 4H4c-.98 0-2 1.22-2 2.5S3 9 4 9zm9-3h-1v1h1c1 0 2 1.22 2 2.5S13.98 12 13 12H9c-.98 0-2-1.22-2-2.5 0-.83.42-1.64 1-2.09V6.25c-1.09.53-2 1.84-2 3.25C6 11.31 7.55 13 9 13h4c1.45 0 3-1.69 3-3.5S14.5 6 13 6z"></path></svg></a>Benchmarking</h2>
<p>Benchmark using this setup:</p>
<ul>
<li>2 different sizes : <strong>512x512</strong> and <strong>768x768</strong></li>
<li>5 point heat stencil</li>
<li><strong><code>double</code></strong> datatype</li>
<li>Iterate until Epsilon (as the sum of all changes across the grid) is less than <strong>10.0</strong> for the 512² gird, and <strong>100.0</strong> for the 768² grid</li>
<li>North/East/South/West boundaries fixed at <strong>1.0, 0.5, 0.0 and -0.5</strong> respectively</li>
<li>All initial values should be set to <strong>0.0</strong></li>
<li>Use the <strong>-O3 and -march=native</strong> compiler flags (not -Ofast)</li>
</ul>
<p><em>Use 1, 2, 4 and 8 processes on a single node of the cluster,
as well as 2, 4 and 8 nodes across the cluster (using 16, 32 and 64 cores/processes respectively).</em></p>
<p><strong>Create a chart which shows the perforamnce scaling across all 7 increasing core counts.</strong></p>
<h2><a href="#unit-testing" aria-hidden="true" class="anchor" id="user-content-unit-testing"><svg aria-hidden="true" class="octicon octicon-link" height="16" version="1.1" viewBox="0 0 16 16" width="16"><path fill-rule="evenodd" d="M4 9h1v1H4c-1.5 0-3-1.69-3-3.5S2.55 3 4 3h4c1.45 0 3 1.69 3 3.5 0 1.41-.91 2.72-2 3.25V8.59c.58-.45 1-1.27 1-2.09C10 5.22 8.98 4 8 4H4c-.98 0-2 1.22-2 2.5S3 9 4 9zm9-3h-1v1h1c1 0 2 1.22 2 2.5S13.98 12 13 12H9c-.98 0-2-1.22-2-2.5 0-.83.42-1.64 1-2.09V6.25c-1.09.53-2 1.84-2 3.25C6 11.31 7.55 13 9 13h4c1.45 0 3-1.69 3-3.5S14.5 6 13 6z"></path></svg></a>Unit Testing</h2>
<p>Your programs and their individual components should be unit tested.</p>
<h2><a href="#lcc2" aria-hidden="true" class="anchor" id="user-content-lcc2"><svg aria-hidden="true" class="octicon octicon-link" height="16" version="1.1" viewBox="0 0 16 16" width="16"><path fill-rule="evenodd" d="M4 9h1v1H4c-1.5 0-3-1.69-3-3.5S2.55 3 4 3h4c1.45 0 3 1.69 3 3.5 0 1.41-.91 2.72-2 3.25V8.59c.58-.45 1-1.27 1-2.09C10 5.22 8.98 4 8 4H4c-.98 0-2 1.22-2 2.5S3 9 4 9zm9-3h-1v1h1c1 0 2 1.22 2 2.5S13.98 12 13 12H9c-.98 0-2-1.22-2-2.5 0-.83.42-1.64 1-2.09V6.25c-1.09.53-2 1.84-2 3.25C6 11.31 7.55 13 9 13h4c1.45 0 3-1.69 3-3.5S14.5 6 13 6z"></path></svg></a>LCC2</h2>
<p>All benchmarks should be performed on the LCC2 cluster. <em>Please make sure that your benchmarking runs do not take more than 5 minutes at most.</em> Shorter runs are preferable.</p>
<h2><a href="#general-note" aria-hidden="true" class="anchor" id="user-content-general-note"><svg aria-hidden="true" class="octicon octicon-link" height="16" version="1.1" viewBox="0 0 16 16" width="16"><path fill-rule="evenodd" d="M4 9h1v1H4c-1.5 0-3-1.69-3-3.5S2.55 3 4 3h4c1.45 0 3 1.69 3 3.5 0 1.41-.91 2.72-2 3.25V8.59c.58-.45 1-1.27 1-2.09C10 5.22 8.98 4 8 4H4c-.98 0-2 1.22-2 2.5S3 9 4 9zm9-3h-1v1h1c1 0 2 1.22 2 2.5S13.98 12 13 12H9c-.98 0-2-1.22-2-2.5 0-.83.42-1.64 1-2.09V6.25c-1.09.53-2 1.84-2 3.25C6 11.31 7.55 13 9 13h4c1.45 0 3-1.69 3-3.5S14.5 6 13 6z"></path></svg></a>General Note</h2>
<p><em>Every</em> member of your group should be ready to explain your methods and findings. All of you should also be able to answer in-depth question about the problem studied.</p>
