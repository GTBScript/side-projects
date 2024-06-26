import os
import telebot
from telebot import types
import openai

# I thought I can use my account on OpenAI with gpt+ subscription, but they are money hungry dudes...
# because i am tired to share my account with my friends :D

TOKEN = ""
bot = telebot.TeleBot(TOKEN)
openai.api_key = ''
sessions = {}

def process_chat_id(message):
    chat_id = message.text
    bot.reply_to(message, f"Continuing chat with ID: {chat_id}")


@bot.message_handler(commands=['start'])
def send_welcome(message):
    markup = types.ReplyKeyboardMarkup(row_width=1, one_time_keyboard=True)
    btn1 = types.KeyboardButton('Start New Chat with GPT-3.5')
    btn2 = types.KeyboardButton('Start New Chat with GPT-4')
    btn3 = types.KeyboardButton('Start New Chat with GPT-4O')
    btn4 = types.KeyboardButton('Start New Chat with DALL-E')
    btn5 = types.KeyboardButton('Enter Chat ID to continue')
    markup.add(btn1, btn2, btn3, btn4, btn5)
    bot.send_message(message.chat.id, "Choose a GPT version or continue a chat:", reply_markup=markup)


@bot.message_handler(func=lambda message: True)
def handle_message(message):
    user_id = message.from_user.id
    if 'Start New Chat with GPT' in message.text:
        version_map = {
            'GPT-3.5': 'gpt-3.5-turbo-0125',
            'GPT-4': 'gpt-4-turbo',
            'GPT-4O': 'gpt-4o',
            'DALL-E': 'dall-e-3'
        }

        friendly_name = message.text.split('with ')[1].strip()
        model_name = version_map.get(friendly_name, 'Model not available')

        if model_name == 'Model not available':
            bot.reply_to(message, "The requested model is not available. Please use /start to select a valid model.")
            return
        try:
            response = openai.ChatCompletion.create(
                model=model_name,  # Make sure the model name is correct e.g., "gpt-3.5-turbo", "text-davinci-002"
                messages=[{"role": "system", "content": "Start a new conversation"}]
            )
            sessions[user_id] = response['id']
            bot.reply_to(message, f"New conversation started with {friendly_name}. Your conversation ID: {response['id']}")
        except Exception as e:
            bot.reply_to(message, str(e))
    elif 'Enter Chat ID to continue' in message.text:
        sent = bot.send_message(message.chat.id, "Please enter the Chat ID:")
        bot.register_next_step_handler(sent, process_chat_id)
    else:
        bot.reply_to(message, "I'm not sure what you mean. Use /start to see options.")


bot.polling()
