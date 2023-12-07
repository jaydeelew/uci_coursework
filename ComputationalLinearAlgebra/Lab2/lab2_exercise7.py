#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 3 - Python
# Student Name:   
# Student ID:   

# ## What to hand in
# Go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download this completed lab as a .py file, and submit it to Gradescope. Make sure the .py file is named lab3_exercise7.py

# In[14]:


import numpy as np
import sympy as sp


# ___
# ## Exercise 7
# (a) Create a linking matrix L which contains the information of which site links to which just as we did in the popularity example. Remember to normalize, and be sure that your input is exact (for example, make sure you enter 1/3 instead of .333 - this is important for part b, since our columns must sum to 1). Save L as a NumPy array.

# In[20]:


L = np.array([[  0, 1/2, 1/3, 1/3, 0],
              [1/4,   0, 1/3,   0, 0],
              [1/4, 1/2,   0, 1/3, 1],
              [1/4,   0,   0,   0, 0],
              [1/4,   0, 1/3, 1/3, 0]])
L = np.transpose(L)


# (b) Use the rref command to find all solutions x to the matrix equation $(L - I)x = 0$. Save the row-reduced form to the variable z7. (If you get something like "Empty matrix: 5-by-0", be sure to double check your answer to part a!).
# 
# 
# Note: Due to floating point approximation errors, you may get an incorrect row-reduced form. You can avoid this by specifying a parameter for rref:
# 
# `z7 = A.rref(iszerofunc=lambda x:abs(x)<10**-10)`

# In[21]:


p = np.array([[0],
              [0],
              [0],
              [0],
              [0]])

I = np.eye(5)
A = np.subtract(L, I)
C = np.concatenate([A, p], axis=1)
D = sp.Matrix(C)
z7 = D.rref(iszerofunc=lambda x:abs(x)<10**-10)
z7


# (c) Whose website has the highest PageRank? Explain your answer, especially in light of any negative
# numbers which may have arisen in part (b). List the remaining websites in order of decreasing PageRank.

# **Type in your response to exercise 7 here:** 

# C Highest Page Rank X_3 has the greatest multiplier
# A
# B
# E
# D

# In practice L is often huge. So a question to mull over is how can we solve $Lr = r$ for problems of huge size? Working in our favor is the fact that that most entries of L are 0. Such an L is called a sparse matrix, and a major branch of mathematics research is exploiting this to find a solution. What do we do? A hint: Gaussian Elimination (row reduction) will not work. Later in these assignments we shall get a glimpse of something that does.

# In[17]:


Acknowledgement = 1

