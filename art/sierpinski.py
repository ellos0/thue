#draws sierpinski triangle

from turtle import *

def main():
    a = input("Pattern Text:\n")
    w = Screen()
    w.bgcolor("blue")
    w.title("sierpinski triangle")
    t = Turtle()

    s = 10
    b = 120

    for c in a:
        if c=='F':
           t.forward(s)
        elif c=='G':
           t.forward(s)
        elif c=='-':
           t.left(b)
        elif c=='+':
           t.right(b)
        else:
          print("?")
    done()

main()