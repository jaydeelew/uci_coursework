#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 6 - Python
# Student Name:   
# Student ID:   

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-4---Python" data-toc-modified-id="ICS-6N-Lab-4---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 6 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-2" data-toc-modified-id="Exercise-2-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 2</a></span></li></ul></li></ul></div>

# ## What to hand in
# For the exercises, go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download the completed exercises as .py files, and submit it to Gradescope. Submit this file under the name lab6_exercise2.py

# Before we start, let's first import numpy.

# In[1]:


import numpy as np


# ___
# ## Exercise 2
# 
# In this exercise we are going to work with the following matrix:

# In[2]:


N = np.array([[.01, .003, 0],
              [.1, .01, 0],
              [0, 0, .005]])


# Use linalg.det() to compute the determinant of $N^{100}$.  Do you think that $N^{100}$ is invertible?  Also use the command to compute the determinant of N.

# In[8]:


det_N100 = np.linalg.det(N ** 100)
det_N100


# In[9]:


det_N = np.linalg.det(N)
det_N


# Now, using the determinant of N as a known quantity, calculate by hand the determinant of $N^{100}$.  Would you now reconsider your answer to the previous question?  Write your final answer in the below variable as either True (yes, it is invertible) or False (no, it is not).
# 
# Hint: look at Remark 4.1 and consider some of the properties of determinants.

# In[11]:


is_it_invertible = 1


# In[ ]:




