import turtle
from tkinter import *
import time
import random
import os

colors = ['red', 'white', 'purple', 'yellow', 'blue', 'orange', 'lime', 'cyan']


def CloseSinglePlayerWindow():
    SinglePlayerMenu.destroy()


def CloseDoublePlayerWindow():
    DoublePlayerMenu.destroy()


def CloseMainMenu():
    MainMenu.destroy()


def CloseAll():
    CloseDoublePlayerWindow()
    CloseSinglePlayerWindow()
    exit(1)


def Entrance():
    global MainMenu

    MainMenu = Tk()
    MainMenu.resizable(0, 0)
    MainMenu.geometry('500x340')
    MainMenu.title('Main Entrance')
    MainMenu.configure(bg='black')

    Label(MainMenu, bg='black').pack()
    Label(MainMenu, bg='black', fg='white', font=('ModeNine', 30), text='Choose the regime').pack()
    Label(MainMenu, bg='black').pack()
    Button(MainMenu, text='1 vs Robot', command=SinglePlayerLaunch, fg='black', font=('ModeNine', 30), width=25,
           height=2).pack()
    Label(MainMenu, bg='black').pack()
    Button(MainMenu, text='1 vs 1', command=DoublePlayerLaunch, fg='black', font=('ModeNine', 30), width=25,
           height=2).pack()
    Label(MainMenu, bg='black').pack()
    Button(MainMenu, text='Bot vs Bot', fg='black', font=('ModeNine', 25), width=17, height=1,
           command=lambda: game('Bot 1', 'Bot 2')).pack()

    MainMenu.mainloop()


def SinglePlayerLaunch():
    global SinglePlayerMenu

    SinglePlayerMenu = Toplevel(MainMenu)
    SinglePlayerMenu.resizable(0, 0)
    SinglePlayerMenu.geometry('500x300')
    SinglePlayerMenu.title('Singleplayer')
    SinglePlayerMenu.configure(bg='black')

    name = StringVar()
    Label(SinglePlayerMenu, bg='black').pack()
    Label(SinglePlayerMenu, text='Write your nickname:', font=('ModeNine', 25), width=30, bg='black',
          fg='white').pack()
    Label(SinglePlayerMenu, bg='black').pack()
    Entry(SinglePlayerMenu, textvariable=name, justify='center', font=('ModeNine', 26), width=30).pack()
    Label(SinglePlayerMenu, bg='black').pack()
    Button(SinglePlayerMenu, text='Fight', font=('ModeNine', 30), width=15, height=1,
           command=lambda: game(name.get(), 'Bot')).pack()
    Label(SinglePlayerMenu, bg='black').pack()
    Button(SinglePlayerMenu, text='Back', font=('ModeNine', 20), width=10, command=CloseSinglePlayerWindow).pack()


def DoublePlayerLaunch():
    global DoublePlayerMenu

    DoublePlayerMenu = Toplevel(MainMenu)
    DoublePlayerMenu.resizable(0, 0)
    DoublePlayerMenu.title('Nicknames')
    DoublePlayerMenu.geometry('500x340')
    DoublePlayerMenu.configure(bg='black')

    player1 = StringVar()
    player2 = StringVar()

    Label(DoublePlayerMenu, bg='black').pack()
    Label(DoublePlayerMenu, text='Write your nicknames below.', font=('ModeNine', 25), fg='white', bg='black').pack()
    Label(DoublePlayerMenu, text='Player A:', font=('ModeNine', 20), fg='white', bg='black').pack()
    Entry(DoublePlayerMenu, textvariable=player1, font=('ModeNine', 20), justify='center').pack()
    Label(DoublePlayerMenu, text='Player B:', font=('ModeNine', 20), fg='white', bg='black').pack()
    Entry(DoublePlayerMenu, textvariable=player2, font=('ModeNine', 20), justify='center').pack()
    Label(DoublePlayerMenu, bg='black').pack()
    Button(DoublePlayerMenu, text='Fight!', font=('ModeNine', 30), width=20,
           command=lambda: game(player1.get(), player2.get())).pack()
    Label(DoublePlayerMenu, bg='black').pack()
    Button(DoublePlayerMenu, text='Back', font=('ModeNine', 20), width=10, command=CloseDoublePlayerWindow).pack()

    DoublePlayerMenu.mainloop()


def game(user1, user2):
    CloseMainMenu()

    global colors

    battlefield = turtle.Screen()
    battlefield.bgcolor('black')
    battlefield.setup(width=1200, height=800)
    battlefield.tracer(0)

    border = turtle.Turtle()
    border.penup()
    border.color('white')
    border.hideturtle()
    border.goto(-550, 300)
    border.pendown()
    border.forward(1100)
    border.right(90)
    border.forward(660)
    border.right(90)
    border.forward(1100)
    border.right(90)
    border.forward(660)

    playerA = turtle.Turtle()
    playerA.speed(0)
    playerA.color('white')
    playerA.shape('square')
    playerA.shapesize(stretch_len=1, stretch_wid=7)
    playerA.penup()
    playerA.goto(-550, 10)

    playerB = turtle.Turtle()
    playerB.speed(0)
    playerB.color('white')
    playerB.shape('square')
    playerB.shapesize(stretch_len=1, stretch_wid=7)
    playerB.penup()
    playerB.goto(550, 10)

    ball = turtle.Turtle()
    ball.speed(0)
    ball.shape('circle')
    ball.color(random.choice(colors))
    ball.penup()
    ball.dx = random.choice([-5, 5])
    ball.dy = random.choice([-5, 5])

    def movePlayerAUp():
        playerA.sety(playerA.ycor() + 40)

    def movePlayerADown():
        playerA.sety(playerA.ycor() - 40)

    def movePlayerBDown():
        playerB.sety(playerB.ycor() - 40)

    def movePlayerBUp():
        playerB.sety(playerB.ycor() + 40)

    battlefield.listen()

    scoreA = 0
    scoreB = 0
    winRange = 5
    status = True

    if user1 == 'Bot 1':
        playerA.dy = 4
    else:
        battlefield.onkeypress(movePlayerAUp, 'w')
        battlefield.onkeypress(movePlayerADown, 's')
        playerA.dy = 0

    if user2 != 'Bot' or user2 != 'Bot 2':
        battlefield.onkeypress(movePlayerBUp, 'Up')
        battlefield.onkeypress(movePlayerBDown, 'Down')
        playerB.dy = 0

    pen = turtle.Turtle()
    pen.color('white')
    pen.penup()
    pen.hideturtle()
    pen.goto(0, 335)
    pen.write('{}: {}    {}: {}'.format(user1, scoreA, user2, scoreB), font=('ModeNine', 30, 'normal'),
              align='center')

    while status:

        battlefield.update()

        ball.setx(ball.xcor() + ball.dx)
        ball.sety(ball.ycor() + ball.dy)

        playerB.sety(playerB.ycor() + playerB.dy)

        if user1 == 'Bot 1':
            if playerA.ycor() < ball.ycor():
                playerA.sety(playerA.ycor() + 8)

            if playerA.ycor() > ball.ycor():
                playerA.sety(playerA.ycor() - 8)

        if user2 == 'Bot' or user2 == 'Bot 2':
            if playerB.ycor() < ball.ycor():
                playerB.sety(playerB.ycor() + 8)

            if playerB.ycor() > ball.ycor():
                playerB.sety(playerB.ycor() - 8)

        if playerB.ycor() > 300:
            playerB.dy *= -1

        if playerB.ycor() < -330:
            playerB.dy *= -1

        if ball.ycor() > 285:
            ball.dy *= -1
            ball.color(random.choice(colors))
            os.system("afplay bounce.wav&")

        if ball.ycor() < -345:
            ball.dy *= -1
            ball.color(random.choice(colors))
            os.system("afplay bounce.wav&")

        if ball.xcor() < -650:
            scoreB += 1
            ball.dy = 0;
            ball.dx = 0
            pen.clear()
            pen.write('{}: {}    {}: {}'.format(user1, scoreA, user2, scoreB), font=('ModeNine', 30, 'normal'),
                      align='center')
            ball.goto(0, 0)
            os.system("afplay {}.wav&".format(random.choice(['criticalhit', 'hit'])))
            ball.color(random.choice(colors))
            ball.dx = random.choice([-5, 5])
            ball.dy = random.choice([-5, 5])

        if ball.xcor() > 620:
            scoreA += 1
            pen.clear()
            pen.write('{}: {}    {}: {}'.format(user1, scoreA, user2, scoreB), font=('ModeNine', 30, 'normal'),
                      align='center')
            ball.dy = 0;
            ball.dx = 0
            ball.goto(0, 0)
            os.system('afplay {}.wav&'.format(random.choice(['criticalhit', 'hit'])))
            ball.color(random.choice(colors))
            ball.dx = random.choice([-5, 5])
            ball.dy = random.choice([-5, 5])

        if (525 < ball.xcor() < 535) and \
                (playerB.ycor() + 85 > ball.ycor() > playerB.ycor() - 85):
            ball.dx *= -1
            ball.color(random.choice(colors))
            os.system('afplay bounce.wav&')

        if (-525 > ball.xcor() > -535) and \
                (playerA.ycor() + 85 > ball.ycor() > playerA.ycor() - 85):
            ball.dx *= -1
            ball.color(random.choice(colors))
            os.system('afplay bounce.wav&')

        if scoreA == winRange or scoreB == winRange:
            status = False
            os.system('afplay kill.wav&')
            stage = 6
            while stage != 0:
                if scoreA == winRange:
                    pen.clear()
                    pen.write('☭ {} wins the game! ☭'.format(user1), font=('ModeNine', 50, 'normal'),
                              align='center')
                    time.sleep(0.5)
                    pen.clear()
                    pen.write('☭ {} wins the game! ☭'.format(user1), font=('ModeNine', 30, 'normal'),
                              align='center')
                    time.sleep(0.5)
                    stage -= 1
                else:
                    pen.clear()
                    pen.write('☭ {} wins the game! ☭'.format(user2), font=('ModeNine', 50, 'normal'),
                              align='center')
                    time.sleep(0.5)
                    pen.clear()
                    pen.write('☭ {} wins the game! ☭'.format(user2), font=('ModeNine', 30, 'normal'),
                              align='center')
                    time.sleep(0.5)
                    stage -= 1
            MainMenu.destroy()


Entrance()
