#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 5 - Python
# Student Name:   
# Student ID:   

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-3---Python" data-toc-modified-id="ICS-6N-Lab-3---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 5 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-5" data-toc-modified-id="Exercise-5-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 5</a></span></li></ul></li></ul></div>

# ## What to hand in
# For the exercises, go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download the completed exercises as .py files, and submit it to Gradescope. Submit this file under the name lab5_exercise5.py

# Before we start, let's first import numpy.

# In[1]:


import numpy as np


# ___
# ## Exercise 5
# 
# (a) Use NumPy to find the inverse of C, and save it to the variable C_inv.
#  
# 

# In[3]:


C = np.array([[5, 10],
             [2, 3]])

C_inv = np.linalg.inv(C)


# In[4]:


C_inv


# (b) Find the inverse of A, and save it to the variable B. Check that it satisfies the definition (A @ B, B @ A)
# 

# In[14]:


A = np.array([[4, 9], 
              [5, 11]])
B = np.linalg.inv(A)

AB = A @ B
BA = B @ A


# In[15]:


B


# In[16]:


AB


# In[17]:


BA


# (c) Multiply A by x by entering the following command:
# 

# In[18]:


x = np.array([[5], 
              [10]])
y = A @ x


# In[19]:


y


# d. Now solve B @ y. Think about why the answer is the way it is.

# In[21]:


By = B @ y


# In[22]:


By

