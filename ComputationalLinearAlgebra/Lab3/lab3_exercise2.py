#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 5 - Python
# Student Name:   
# Student ID:   

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-3---Python" data-toc-modified-id="ICS-6N-Lab-3---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 5 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-2" data-toc-modified-id="Exercise-2-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 2</a></span></li></ul></li></ul></div>

# ## What to hand in
# For the exercises, go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download the completed exercises as .py files, and submit it to Gradescope. Submit this file under the name lab5_exercise2.py

# Before we start, let's first import numpy.

# In[1]:


import numpy as np


# ___
# ## Exercise 2
# 
# Given the following A, B, and x,

# In[2]:


A = np.array([[1, 2, 0],
              [2, 1, 2], 
              [0, 2, 1]])
B = np.array([[3, 0, 3], 
              [1, 5, 1],
              [1, 1, 3]])
x = np.array([[1],
              [2], 
              [3]])


# (a) Compute the following expression: $C = (2A^{2}B + 3A^{T})^{2}$
# 
# Hint: use `np.linalg.matrix_power` 

# In[3]:


C = np.linalg.matrix_power((2*np.linalg.matrix_power(A, 2)@B + 3*np.transpose(A)), 2)


# In[4]:


C


# (b) Compute $D = Cx$

# In[5]:


D = C@x


# In[6]:


D


# In[7]:


D

