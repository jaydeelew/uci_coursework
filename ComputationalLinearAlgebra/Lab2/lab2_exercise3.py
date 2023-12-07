#!/usr/bin/env python
# coding: utf-8

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-2---Python" data-toc-modified-id="ICS-6N-Lab-2---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 3 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-3" data-toc-modified-id="Exercise-3-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 3</a></span></li></ul></li></ul></div>

# # ICS 6N Lab 3 - Python
# Student Name:   
# Student ID:   

# ## What to hand in
# Go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download this completed lab as a .py file, and submit it to Gradescope. Make sure the .py file is named lab3_exercise3.py

# In[1]:


import numpy as np
import sympy as sp


# ___
# ## Exercise 3
# Consider the following homogeneous system of equations:
# 
# $x_{1} - 3x_{2} + 2x_{3} = 0$  
# $-2x_{1} + 6x_{2} - 4x_{3} = 0$  
# $4x_{1} - 12x_{2} + 8x_{3} = 0$  
# 
# By using the rref command, save the solution to this system of equations to the variable z3. How many free variables are required?

# In[8]:


A = sp.Matrix([[1, -3,  2, 0],
               [-2, 6, -4, 0],
               [4, -12,  8, 0]])
z3 = A.rref()
z3


# **How many free variables are required? Type in your response to exercise 3 in the below variable:** 
# 

# In[10]:


free_variables_required = 2


# In[11]:


free_variables_required

