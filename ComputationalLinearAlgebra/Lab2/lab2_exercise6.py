#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 3 - Python
# Student Name:   
# Student ID:   

# ## What to hand in
# Go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download this completed lab as a .py file, and submit it to Gradescope. Make sure the .py file is named lab3_exercise6.py

# In[1]:


import sympy as sp
import numpy as np


# ___
# ## Exercise 6
# (a) Enter the matrices M and I into Python.
# 
# ` M = [[0.25, 0.15, 0.25, 0.18, 0.20],
#      [0.15, 0.28, 0.18, 0.17, 0.05],
#      [0.22, 0.19, 0.22, 0.22, 0.10],
#      [0.20, 0.15, 0.20, 0.28, 0.15],
#      [0.18, 0.23, 0.15, 0.15, 0.50]]
# `
# 
# `  I = np.eye(5)`
# 
# Note that the command "np.eye(n)" creates an n x n matrix with 1's on the diagonal and 0's elsewhere.
# Write both in NumPy format.

# In[21]:


M = [[0.25, 0.15, 0.25, 0.18, 0.20],
     [0.15, 0.28, 0.18, 0.17, 0.05],
     [0.22, 0.19, 0.22, 0.22, 0.10],
     [0.20, 0.15, 0.20, 0.28, 0.15],
     [0.18, 0.23, 0.15, 0.15, 0.50]]

I = np.eye(5)


# (b) Use SymPy to row reduce the augmented matrix $[M - I | p]$ and save the row-reduced form to the variable z6.   
# Hint: To add another column to a matrix, use `np.concatenate([A, b], axis=1)`  
# What is the general solution to $(M - I)p = 0$?

# In[23]:


p = np.array([[1],
              [1],
              [1],
              [1],
              [1]])

A = np.subtract(M, I)
C = np.concatenate([A, p], axis=1)
D = sp.Matrix(C)
z6 = D.rref()


# (c) What are the highest and the lowest priced commodities in Matrixville? List the inhabitants of this
# charming town in order of increasing income. If our friend Bob makes 40,000 dollars per year, what are the incomes for the rest of the inhabitants? With all the moonshine that Bob drinks, do you think he will live long enough to enjoy his good life?

# Highest: Moonshine
# Lowest: Clothing
# 
# Farmer: $32,099
# Tailor: $23,675
# Carpenter: $28,432
# Coal Miner: $30,080
# Slacker Bob: $40,000
# 
# Bob should drink a lot of water and keep his electrolytes up.
