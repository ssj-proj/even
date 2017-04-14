import gym
from evepy import even

#init eve
#init openai

#get output - handle no output
#sent input
#manage reward and mutations


env = gym.make('CartPole-v0')
print(env.action_space)
observation=env.reset()
for _ in range(1000):
    env.render()
    print(observation)
    act = env.action_space.sample()

    #observation, reward, done, info = env.step(0) # take a random action
    print(observation)
