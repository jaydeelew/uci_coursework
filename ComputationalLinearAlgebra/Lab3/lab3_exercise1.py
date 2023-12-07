#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 5 - Python
# Student Name:   
# Student ID:   

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-3---Python" data-toc-modified-id="ICS-6N-Lab-3---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 5 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#Exercise-1" data-toc-modified-id="Exercise-1-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>Exercise 1</a></span></li></ul></li></ul></div>

# ## What to hand in
# For the exercises, go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download the completed exercises as .py files, and submit it to Gradescope. Submit this file under the name lab5_exercise1.py

# Before we start, let's first import numpy.

# In[2]:


import numpy as np


# ___
# ## Exercise 1
# 
# Can you think of any 2x2 matrices D for which CD = DC for all 2x2 matrices C?  Give 2 different examples of such matrices D1 and D2. Make sure they are in NumPy format.

# In[9]:


D1 = np.array([[1, 0], [0, 1]])
D2 = np.array([[0, 0], [0, 0]])

