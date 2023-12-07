#!/usr/bin/env python
# coding: utf-8

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-2---Python" data-toc-modified-id="ICS-6N-Lab-2---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 3 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-1" data-toc-modified-id="Exercise-1-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 1</a></span></li></ul></li></ul></div>

# # ICS 6N Lab 3 - Python
# Student Name:   
# Student ID:   

# ## What to hand in
# Go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download this completed lab as a .py file, and submit it to Gradescope. Make sure the .py file is named lab3_exercise1.py

# In[1]:


import numpy as np


# ___
# ## Exercise 1
# a. Consider the system of equations:
# 
# $2x_{1} + x_{2} + 5x_{3} = -1$  
# $x_{1} + 6x_{3} = 2$  
# $-6x_{1} + 2x_{2} + 4x_{3} = 3$
# 
# Write this system of equations into a matrix equation of the form $Cx = d$ for $C$ and $d$ in NumPy format below:

# In[2]:


C = np.array([[2, 1,  5],
              [1,  0,  6],
              [-6,  2,  4]])
d = np.array([[-1],
              [2],
              [3]])


# b. Use the np.linalg.solve() function to solve for x and verify your solution.

# In[3]:


x = np.linalg.solve(C, d)


# c. We would expect to get the column vector d in Python if we multiplied C and x, right? In other
# words, C · x - d should be zero. Enter this equation into Python:  
# `z1 = C @ x - d`  (Note, we use @ here for matrix multiplication. * is for element-wise multiplication)  
# What do you get?

# In[17]:


z1 = C @ x - d


# In[18]:


z1


# **Remark 2.2** The discrepancy in part (c) of the above exercise is simply due to rounding error. You'll notice that the error is a vector multiplied by a very small number, one on the order of $10^{-15}$. But why is there any error at all? After all, solving by row reduction gave very nice numbers, right? The reason lies in the way Python stores numbers. In this calculation Python represents numbers in "floating point form", which means it represents them in scientific notation to 10^(-14) accuracy. Thus when you see 10^(-14) printed in calculations, it is equivalent to zero. 
# 
# There is a drawback, however, to solving systems of equations using the command "np.linalg.solve()". Let us explore that further.
# 

# ___
