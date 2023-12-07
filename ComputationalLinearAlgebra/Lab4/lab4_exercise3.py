#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 4 - Python
# Student Name:   
# Student ID:   

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-4---Python" data-toc-modified-id="ICS-6N-Lab-4---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 6 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-3" data-toc-modified-id="Exercise-3-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 3</a></span></li></ul></li></ul></div>

# ## What to hand in
# For the exercises, go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download the completed exercises as .py files, and submit it to Gradescope. Submit this file under the name lab6_exercise3.py

# Before we start, let's first import numpy.

# In[2]:


import numpy as np


# ___
# ## Exercise 3
# 
# a) Find the eigenvectors and corresponding eigenvalues of V.  Assign them to matrices P and D respectively.

# In[3]:


V = np.array([[9, -4, -2, 0],
              [-56, 32, -28, 44],
              [-14, -14, 6, -14],
              [42, -33, 21, -45]])


# In[4]:


D, P = D, P = np.linalg.eig(V)


# In[5]:


D


# In[6]:


P


# b) Determine if V is invertible by looking at the eigenvalues. Write your answer as either True (yes, it is invertible) or False (no, it is not invertible) in the below variable.

# In[7]:


is_it_invertible = 1


# c) Use NumPy to evaluate $P^{-1}VP$.  Think about what stands out from the output.

# In[13]:


Pinv_VP = np.linalg.inv(P) @ V @ P


# In[14]:


Pinv_VP

