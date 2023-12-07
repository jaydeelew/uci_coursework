#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 5 - Python
# Student Name:   
# Student ID:   

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-3---Python" data-toc-modified-id="ICS-6N-Lab-3---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 5 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-4" data-toc-modified-id="Exercise-4-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 4</a></span></li></ul></li></ul></div>

# ## What to hand in
# For the exercises, go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download the completed exercises as .py files, and submit it to Gradescope. Submit this file under the name lab5_exercise4.py

# Before we start, let's first import numpy.

# In[18]:


import numpy as np


# ___
# ## Exercise 4
# 
# (a) Recall from before in the incidence matrices section, we got the matrix $A$. 
# 

# In[19]:


A = np.array([[0, 1, 0, 0, 0, 0],
              [1, 0, 1, 1, 1, 1],
              [0, 1, 0, 1, 1, 0],
              [1, 1, 1, 0, 1, 0],
              [0, 0, 1, 1, 0, 1],
              [0, 1, 0, 1, 1, 0]])


# From there, state the number of ways to get from San Diego to Moscow with exactly 3 stopovers.  Simply solve $A^{4}$. Hint: use `np.linalg.matrix_power` and read off the entry in the first row and fifth column. Write how many stopovers you find in num_stopovers_SD_Moscow.

# In[33]:


B = np.linalg.matrix_power(A, 4)


# In[34]:


num_stopovers_SD_Moscow = 10


# (b) Find the number of ways to get from San Francisco to Chicago with at most 4 stopovers. (Note, this is not the same as finding the number of ways with exactly 4 stopovers!)
# 
# Write your answer in num_stopovers_SF_Chicago.

# In[35]:


num_stopovers_SF_Chicago = 142

