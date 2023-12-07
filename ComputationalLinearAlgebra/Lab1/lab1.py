#!/usr/bin/env python
# coding: utf-8

# # ICS 6N Lab 1 - Python
# Student Name: Jack Lewis
# Student ID: 26197631

# <h1>Table of Contents<span class="tocSkip"></span></h1>
# <div class="toc"><ul class="toc-item"><li><span><a href="#ICS-6N-Lab-1---Python" data-toc-modified-id="ICS-6N-Lab-1---Python-1"><span class="toc-item-num">1&nbsp;&nbsp;</span>ICS 6N Lab 1 - Python</a></span><ul class="toc-item"><li><span><a href="#What-to-hand-in" data-toc-modified-id="What-to-hand-in-1.1"><span class="toc-item-num">1.1&nbsp;&nbsp;</span>What to hand in</a></span></li><li><span><a href="#About-Python" data-toc-modified-id="About-Python-1.2"><span class="toc-item-num">1.2&nbsp;&nbsp;</span>About Python</a></span></li><li><span><a href="#Variables-in-Python" data-toc-modified-id="Variables-in-Python-1.3"><span class="toc-item-num">1.3&nbsp;&nbsp;</span>Variables in Python</a></span></li><li><span><a href="#Exercise-1" data-toc-modified-id="Exercise-1-1.4"><span class="toc-item-num">1.4&nbsp;&nbsp;</span>Exercise 1</a></span></li><li><span><a href="#Exercise-2" data-toc-modified-id="Exercise-2-1.5"><span class="toc-item-num">1.5&nbsp;&nbsp;</span>Exercise 2</a></span></li><li><span><a href="#Matrices-in-Python" data-toc-modified-id="Matrices-in-Python-1.6"><span class="toc-item-num">1.6&nbsp;&nbsp;</span>Matrices in Python</a></span></li><li><span><a href="#Exercise-3" data-toc-modified-id="Exercise-3-1.7"><span class="toc-item-num">1.7&nbsp;&nbsp;</span>Exercise 3</a></span></li><li><span><a href="#Matrix-operations-and-manipulations" data-toc-modified-id="Matrix-operations-and-manipulations-1.8"><span class="toc-item-num">1.8&nbsp;&nbsp;</span>Matrix operations and manipulations</a></span></li><li><span><a href="#Exercise-4" data-toc-modified-id="Exercise-4-1.9"><span class="toc-item-num">1.9&nbsp;&nbsp;</span>Exercise 4</a></span></li><li><span><a href="#Random-matrices" data-toc-modified-id="Random-matrices-1.10"><span class="toc-item-num">1.10&nbsp;&nbsp;</span>Random matrices</a></span></li><li><span><a href="#Exercise-5" data-toc-modified-id="Exercise-5-1.11"><span class="toc-item-num">1.11&nbsp;&nbsp;</span>Exercise 5</a></span></li><li><span><a href="#Exercise-6" data-toc-modified-id="Exercise-6-1.12"><span class="toc-item-num">1.12&nbsp;&nbsp;</span>Exercise 6</a></span></li></ul></li></ul></div>

# ## What to hand in
# Go to File on the top left corner of Jupyter Notebook, and hover over the Download As... option. Download this completed lab as a .py file, and submit it to Gradescope. Make sure the .py file is named lab1.py

# ## About Python
# Linear algebra is used in every branch of mathematics and is thus a crucial part of it. Linear algebra also has applications in practically every field of natural and social sciences. Physics, chemistry, biology, computer science, economics, and sociology have all benefited from the advent of linear algebra. In the lab sections this quarter you will see a few applications of linear algebra to some of these areas.
# 
# Problems that arise in these fields involve raw data, so professionals in these fields have come to depend on computers as an essential tool. Programming languages like Python can help in performing tedious calculations so that you can spend less time crunching numbers. It turns out that some Python libraries are especially well suited for working with matrices and performing various algorithmic routines that come up in linear algebra. In this lab we will start to get acquainted with Python in the Jupyter Notebook environment and learn how it handles matrices via the NumPy library.

# Down here, we will be importing the NumPy library for further use as we go through this lab. For ease of use, we can import specific functions, such as cos, sin, and exponentiation as Python functions.
# 
# To run this cell, and other cells throughout the notebook, you can use the shortcut **SHIFT+ENTER**.

# In[1]:


import numpy as np


# ## Variables in Python
# Python can function as a very powerful calculator you can use on your computer. For our
# purpose, that is not far from the mark. To define variables, simply type in the variable you wish to define, then an equal sign, and finally the value you wish to assign to that variable. So, if we want to assign the value of 5 to the variable x, we type the following line and press Shift+Enter (or Shift+Return):

# In[3]:


x = 5


# To check the value of x, you can simply type

# In[4]:


x


# Unless we later redefine x, every time we use "x" Python will use its assigned value 5 when doing a calculation.
# For example, we can define the following variables by their place in the alphabet:

# In[8]:


l=12
i=9
n=14
e=5
a=1
r=18
g=7
b=2
l, i, n, e, a, r, g, b


# We have just assigned specific values to the given letters.
# 
# It is also possible to assign values to more than just single letters using Python. It is sometimes easier to define something with a specific name rather than a single letter. For example, we can define

# In[9]:


UCIrvine = 92697
UCIrvine


# 92697 is the zip code of UCI. Notice that there are no spaces in variable names. (We cannot define a
# variable called "UC Irvine".) Also, when defining something, Python is case-sensitive so if we accidentally type "ucirvine" and hit return, we will get an error message as seen here.

# In[10]:


#ucirvine
# You can also write comments within the cells using the # symbol. 
# To submit this assignment, comment out the above ucirvine variable by putting a # in front of it.
# for example: #ucirvine


# Using the variables we just defined, we can do some calculations and save them as specific names.

# In[11]:


linearalgebra = l+i+n+e+a+r+a+l+g+e+b+r+a
angle = a+n+g+l+e


# In[12]:


linearalgebra


# In[13]:


angle


# ___
# ## Exercise 1
# Define the letters for **python** to be the number corresponding to their place in the alphabet.
# Then set the variable named **python**, equal to the sum of the letters.

# In[14]:


p=16
y=25
t=20
h=8
o=15
n=14

python = p+y+t+h+o+n
python


# **Hints and Tips**  
# If you ever get stuck or need more info on a specific NumPy function, an excellent way to get help is by visiting the NumPy documentation at https://numpy.org/doc/1.17/. One great way to make use of this resource is the quick search button on the left side; you can type in any function in NumPy and it will show up with documentation on what it takes in, and some example use cases.
# 
# To call on a function from the NumPy library, you will want to use np.**name**, where **name** is the name of the function. For example, if you wanted to call numpy's cosine function on an input of 3 radians, you would use np.cos(3).

# ___
# ## Exercise 2
# Syntax is very important in making sure your script works. Fix the code below so it runs. It should be the equivalent of 
# $25 - (100 - 7e^{5 + cos(pi/3)})$

# In[16]:


z = 25-(100-7*np.exp(5+np.cos(np.pi/3)))


# ## Matrices in Python
# The first thing to learn when dealing with matrices is how to construct a matrix (or vector) using NumPy.
# 
# To define a matrix using NumPy, we use the square brackets "[ ]". "[" tells NumPy we are starting to
# create a matrix and "]" tells NumPy that we have finished with our construction. The entries of the matrix should be entered in rows from left to right. To separate entries we can insert a comma ",". 
# 
# Once we finish with a given row and want to move to the next, we can make the next row by repeating the same process again through using square brackets to create another row. Each row will be separated by a comma.
# 
# You can collect these rows into a matrix by encasing these rows in a final set of square brackets. 
# 
# However, to set these matrices in a form usable by NumPy, you will need to convert this constructed matrix into a NumPy array object. This can be easily done by using the **np.array(A)** function, where **A** is the matrix you want to convert to a NumPy array.

# In[17]:


A = [[2, 1],
     [4, 3]]
A = np.array(A)
A


# ___
# ## Exercise 3
# Input the following matrix as a NumPy array in the fibonacci variable.
# 
# $$fibonacci = \begin{bmatrix} 1 & 1 & 2 & 3 \\ 5 & 8 & 13 & 21 \\ 34 & 55 & 89 & 144 \\ 233 & 377 & 610 & 987 \end{bmatrix}$$
# 

# In[20]:


fibonacci = [[1, 1, 2, 3,],
     [5, 8, 13, 21],
     [34, 55, 89, 144],
     [233, 377, 610, 987]]
fibonacci = np.array(fibonacci)
fibonacci


# ## Matrix operations and manipulations
# Sometimes we need to get at certain parts of a matrix only, maybe just a certain row or even a single entry.
# 
# This is done with regular parentheses, "(" and ")". For example, to see the (1,2) entry of the matrix A above (i.e. the entry in the first row and second column), we use the command

# In[23]:


A[0, 1]


# **NOTE: In Python, indexing starts at 0, so to see the entry (1, 2), you will want to access it as [0, 1].**

# We also can use the colon ":" to mean 'all', as in the command

# In[25]:


A[1, :]


# which will give us the entire second row of the matrix A. The colon can also be used to represent a range of rows or columns: the command

# In[26]:


fibonacci[1:4, 0]


# will give us the entries of Fibonacci from the second through fourth rows in the first column.

# ___
# ## Exercise 4
# Using the commands introduced above, construct a new matrix (call it whatever you'd like) from fibonacci that consists of the last two rows and the two middle columns of fibonacci. (So it will be a 2x2 matrix.) Save the result in the new_fibonacci variable.

# In[27]:


B = fibonacci[2:5, 1:3]
B


# ## Random matrices
# From time to time, we will be working with random matrices in this class. However, when you are asked to create a random matrix, this does not mean you should just create a matrix using numbers that pop up in your head. Instead, we will let NumPy do the "thinking". To create random matrices, we use the **np.random.rand()** command.
# 
# Notice the parentheses after the command. When using a command that requires some type of input, that
# input must be put in parentheses.

# In[28]:


np.random.rand(3, 3)


# and hitting return tells NumPy to create a random 3 x 3 matrix whose entries are decimal values between 0 and 1.

# ___
# ## Exercise 5
# Create 3 random 4 x 4 matrices A, B, and C and calculate the following value. (You should store each of
# them in a variable with some name of your choosing.) In theory, do you expect the answer to be a zero
# matrix? Is it in practice?
# 
# (A + B + C) – (A + C + B)

# In[31]:


A = np.random.rand(4, 4)
B = np.random.rand(4, 4)
C = np.random.rand(4, 4)

D = (A + B + C) - (A + C + B)
D


# In[32]:


E = (A + B + C) - (A + B + C)
E


# Enter your response here: Yes, I expected a zero matrix for both. D above did not zero out, however E did.

# ___
# ## Exercise 6
# How do you find $sin^{-1}(1)$ using NumPy? (Hint: Start by looking at the NumPy documentation). Save the value into the variable below.

# In[33]:


np.arcsin(1) 

