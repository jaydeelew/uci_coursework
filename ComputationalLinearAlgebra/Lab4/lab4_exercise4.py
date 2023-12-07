#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 6 - Python
# Student Name:   
# Student ID:   

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-4---Python" data-toc-modified-id="ICS-6N-Lab-4---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 6 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-4" data-toc-modified-id="Exercise-4-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 4</a></span></li></ul></li></ul></div>

# ## What to hand in
# For the exercises, go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download the completed exercises as .py files, and submit it to Gradescope. Submit this file under the name lab6_exercise4.py

# Before we start, let's first import numpy.

# In[1]:


import numpy as np


# ___
# ## Exercise 4
# 
# a) Find an invertible matrix P and a diagonal matrix D such that $PDP^{-1} = F$
# 
# Hint: You can convert D into a diagonal matrix using np.diag()

# In[2]:


F = np.array([[0, 1], 
              [1, 1]])


# In[3]:


D, P =  np.linalg.eig(F)
D = np.diag(D)


# In[4]:


D


# In[5]:


P


# b) Compute $F^{10}$ and $PD^{10}P^{-1}$

# In[6]:


F10 = np.linalg.matrix_power(F, 10)
F10


# In[7]:


P_D10_Pinv = P @ np.linalg.matrix_power(D, 10) @ np.linalg.inv(P)
P_D10_Pinv


# c) Let $f = (1, 1)^{T}$ Compute $Ff, F^{2}f, F^{3}f, F^{5}f$. Do you notice something about the pattern?

# In[34]:


f = np.array([[1],
              [1]])
Ff = F @ f


# In[36]:


Ff


# In[37]:


F2f = np.linalg.matrix_power(F, 2) @ f
F2f


# In[39]:


F3f = np.linalg.matrix_power(F, 3) @ f
F3f


# In[40]:


F5f = np.linalg.matrix_power(F, 5) @ f
F5f


# d) Given a sequence of numbers {1, 1, 2, 3, 5, 8, 13, ....} where each term is the sum of the previous two, find the 30th term of this sequence.  (If you are stuck, read the remark below)

# In[33]:


term_30 = 832040


# **Remark 4.3**  The sequence in the above exercise is called a Fibonacci sequence.  It has many interesting properties and appears often in nature.  The above problem points in the direction of how to find an explicit formula for the nth term in this sequence (it is not obvious a priori that such a formula must even exist).  To obtain this formula simply note that if we let
# 
# $f_{0} = f_{1} = 1$ and $f_{n+2} = f_{n+1} + f_{n}$ be our Fibonacci sequence and let
# 
# $f = (f_{0}, f_{1})^{T} = (1, 1)^{T}$
# 
# $Ff = (f_{1}, f_{0} + f_{1})^{T} = (f_{1} + f_{2})^{T}$
# 
# $F^{2}f = F(Ff) = (f_{2}, f_{1} + f_{2})^{T} = (f_{2} + f_{3})^{T}$
# 
# $F^{n}f = (f_{n}, f_{n+1})^{T}$
# 
# Thus, to get the general formula we need to compute $F_{n}$, (which is done by computing $PD^{n}P^{-1}$), multiply it by the vector $(1, 1)^{T}$ and read off the first row of the resulting vector.  If you like, you may perform these calculations by hand at your leisure and derive an interesting formula for the nth Fibonacci number involving the golden ratio.

# In[ ]:




