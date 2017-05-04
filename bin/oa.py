#!/usr/bin/python
import gym,threading
from evepy import even

#init eve
#init openai

#get output - handle no output
#sent input
#manage reward and mutations

eve = even()
eve.test()
eve.start()
#quit()
#steve_thread.halt()
print("STARTED EVE")
env = gym.make('CartPole-v0')
observation=env.reset()
eve.set_oai_work2(observation,4)
for _ in range(1000):
    env.render()
    output = eve.get_work()
    if output==-1:
        output=0#if no output from net - default action
    print("output"+str(output))
    observation, reward, done, info = env.step(output) # take a random action
    eve.set_oai_work2(observation,4)
    print(observation[0])
    print("reward"+str(reward))
