#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 6 - Python
# Student Name:   
# Student ID:   

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-4---Python" data-toc-modified-id="ICS-6N-Lab-4---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 6 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-1" data-toc-modified-id="Exercise-1-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 1</a></span></li></ul></li></ul></div>

# ## What to hand in
# For the exercises, go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download the completed exercises as .py files, and submit it to Gradescope. Submit this file under the name lab6_exercise1.py

# Before we start, let's first import numpy.

# In[1]:


import numpy as np


# ___
# ## Exercise 1
# 
# Use NumPy to compute the determinants of the following matrices:
# 
# $A + B, A - B, AB, A^{-1}, B^{T}$
# 
# and save the results in their respective variables.

# In[5]:


A = np.array([[8, 11, 2, 8], 
              [0, -7, 2, -1],
              [-3, -7, 2, 1],
              [1, 1, 2, 4]])

B = np.array([[1, -2, 0, 5], 
              [0, 7, 1, 5], 
              [0, 4, 4, 0], 
              [0, 0, 0, 2]])


# In[12]:


A_plus_B = np.linalg.det(A + B)
A_plus_B


# In[13]:


A_minus_B = np.linalg.det(A - B)
A_minus_B


# In[14]:


AB = np.linalg.det(A @ B)
AB


# In[15]:


A_inv = np.linalg.det(np.linalg.inv(A))
A_inv


# In[18]:


B_transpose = np.linalg.det(np.transpose(B))
B_transpose


# Look at the above matrix results and take some time to think: which of the above matrices would not be invertible? 
# 
# Now we know the determinants of A and B, but suppose that we lost our original matrices A and B. Think about which determinants in part (a) will we still be able to recover, even without having A or B at hand.
# 
# **Remark 4.1**  The main use of determinants in this class relates to the idea of invertibility.  When you use NumPy for that purpose, you have to understand that the program introduces rounding errors.  Therefore, there is a possibility that a matrix may appear to have zero determinant and yet be invertible.  This only applies to matrices with non-integer entries.  The above matrices don't fall into this category as all their entries are integers.

# In[ ]:




