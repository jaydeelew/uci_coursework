#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 6 - Python
# Student Name:   
# Student ID:   

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-4---Python" data-toc-modified-id="ICS-6N-Lab-4---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 6 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-5" data-toc-modified-id="Exercise-5-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 5</a></span></li></ul></li></ul></div>

# ## What to hand in
# For the exercises, go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download the completed exercises as .py files, and submit it to Gradescope. Submit this file under the name lab6_exercise5.py

# Before we start, let's first import numpy.

# In[3]:


import numpy as np


# ___
# ## Exercise 5
# From the previous exercise you probably observed that the results seem to stabilize the further into the future we go.  Let us try to explain this mathematically.
# 
# a) Find matrices Q and D such that $QDQ^{-1} = P$
# Convert D into a diagonal matrix.

# In[4]:


P = np.array([[0.8100, 0.0800, 0.1600, 0.1000],
              [0.0900, 0.8400, 0.0500, 0.0800],
              [0.0600, 0.0400, 0.7400, 0.0400],
              [0.0400, 0.0400, 0.0500, 0.7800]])

x0 = np.array([48.56, 51.01, 0.0013, 0.0030])


# In[5]:


D, Q = np.linalg.eig(P)
D = np.diag(D)


# In[6]:


D


# In[7]:


Q


# b) Find $\lim_{n\to\infty} D^{n}$ as n gets very large. Since we can't use infinity, we can just go with some arbitrary large value like 99999. Save this value to L. What does the first entry in L converge to? Write your answer in L_converges_to.

# In[8]:


L = np.linalg.matrix_power(D, 99999)
L


# In[1]:


L_converges_to = 1


# c) Multiply your answer by $Q$ and $Q^{-1}$ to compute $P_{\infty} = \lim_{n\to\infty} P^{n}$. Store the answer in a variable called Pinf.

# In[9]:


Pinf = Q @ L @ np.linalg.inv(Q)
Pinf


# Notice that $P_{\infty}x_{0} = P_{inf} * x_{0}$, where $x_{0}$ is the same as in part (a) of the review exercises. Think about how your answer compare to part (b) of the review exercise from last time.

# d) Now make up any Numpy vector y in $R^{4}$ whose entries sum to 100. Compute $P_{\infty}y$ and compare it to part (c).  Think about how the initial distribution y of the electorate seems to affect the distribution in the long term.  By looking at the matrix $P_{\infty}$, think about a possible mathematical explanation.

# In[10]:


y = np.array([[4, 8, 16, 2],
              [0, 0, 1, 4],
              [6, 9, 4, 7],
              [25, 1, 4, 9]])
y


# In[11]:


Pinf_y = Pinf @ y
Pinf_y


# In[ ]:




