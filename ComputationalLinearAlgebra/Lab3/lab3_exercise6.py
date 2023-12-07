#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 5 - Python
# Student Name:   
# Student ID:   

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-3---Python" data-toc-modified-id="ICS-6N-Lab-3---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 5 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-6" data-toc-modified-id="Exercise-6-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 6</a></span></li></ul></li></ul></div>

# ## What to hand in
# For the exercises, go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download the completed exercises as .py files, and submit it to Gradescope. Submit this file under the name lab5_exercise6.py

# Before we start, let's first import numpy.

# In[1]:


import numpy as np


# ___
# ## Exercise 6
# 
# From that lab, we have given that
#  
# 

# In[3]:


P = np.array([[0.8100, 0.0800, 0.1600, 0.1000],
              [0.0900, 0.8400, 0.0500, 0.0800],
              [0.0600, 0.0400, 0.7400, 0.0400],
              [0.0400, 0.0400, 0.0500, 0.7800]])

x0 = np.array([48.56, 51.01, 0.0013, 0.0030])


# (a) What will the party distribution vector be 3, 7, and 10 presidential elections from now? Save your answer in the given respective variables.
# 

# In[4]:


election3 = np.linalg.matrix_power(P, 3) @ x0
election7 = np.linalg.matrix_power(P, 7) @ x0
election10 = np.linalg.matrix_power(P, 10) @ x0


# In[5]:


election3


# In[6]:


election7


# In[7]:


election10


# (b) What will be the results 30, 60, 100 elections from now?

# In[8]:


election30 = np.linalg.matrix_power(P, 30) @ x0
election60 = np.linalg.matrix_power(P, 60) @ x0
election100 = np.linalg.matrix_power(P, 100) @ x0


# In[9]:


election30


# In[10]:


election60


# In[11]:


election100


# As you look at the results, think about how much different is $x_{30}$ from $x_{60}$ from $x_{100}$. What kind of trends are you noticing with $x_{k}$ as k gets big.
