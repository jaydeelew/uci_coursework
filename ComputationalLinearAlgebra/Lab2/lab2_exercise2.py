#!/usr/bin/env python
# coding: utf-8

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-2---Python" data-toc-modified-id="ICS-6N-Lab-2---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 3 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-2" data-toc-modified-id="Exercise-2-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 2</a></span></li></ul></li></ul></div>

# # ICS 6N Lab 3 - Python
# Student Name:   
# Student ID:   

# ## What to hand in
# Go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download this completed lab as a .py file, and submit it to Gradescope. Make sure the .py file is named lab3_exercise2.py

# In[13]:


import numpy as np


# ___
# ## Exercise 2
# Consider the following system of equations:
# 
# $-10x_{1} + 4x_{2} = 0$  
# $15x_{1} - 6x_{2} = 0$
# 
# As you did in the previous exercise, enter the corresponding matrix E and the column vector f into
# Python. Then type in  
# 
# `z2 = np.linalg.solve(E, f)`  
# 
# Note the strange output. Why is this so? Now go ahead and solve this system by hand. How
# many free variables do you need to write down the solution? Based on your answer, can you explain why you got the error message when trying to use the "z2 = np.linalg.solve(E, f)" command? One free variable, infinite solutions.

# In[14]:


E = np.array([[-10, 4],
              [15, -6]])
f = np.array([[0],
              [0]])


# In[15]:


err = None  # Leave this alone.
try:  # This try-except statement is to stop Python from crashing.
    z2 = np.linalg.solve(E, f)
except Exception as err_var:
    err = err_var  # Output any errors


# In[16]:


err

