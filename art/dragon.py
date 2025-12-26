#draws dragon curve

from turtle import *

def main():
  a = input("Pattern Text:\n")
  w = Screen()
  w.bgcolor("green")
  w.title("Dragon curve fractal")
  t = Turtle()
  
  s = 10

  for c in a:
    if c=='F':
      t.forward(s)
    elif c=='G':
      t.forward(s)
    elif c=='+':
      t.left(90)
    elif c=='-':
      t.right(90)
    else:
      print("?")
  done()

main()
