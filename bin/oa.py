#!/usr/bin/python
import gym,threading, time
from evepy import even

#init eve
#init openai

#get output - handle no output
#sent input
#manage reward and mutations

eve = even()
eve.test()
eve.start()
wait_time = eve.get_step_wait()
#quit()
#steve_thread.halt()
print("Started EVE")
env = gym.make('CartPole-v0')
observation=env.reset()
eve.set_oai_work2(observation,4)
i = 0
for _ in range(1000):
    i=i+1
    env.render()
    output = eve.get_work()
    print("Net Out: "+str(output))
    if output==-1:
        output=0#if no output from net - default action
    #print("output"+str(output))
    observation, reward, done, info = env.step(output) # take a random action
    for i in range(4):
        observation[i]=observation[i]*100

    eve.set_oai_work2(observation,4)
    #print(i)
    time.sleep(wait_time/1000000)
    #print("reward"+str(reward))
