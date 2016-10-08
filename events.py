#!/usr/bin/env python

'''
events.py - subscribe to all events and print them to stdout
'''
import ESL
import redis

con = ESL.ESLconnection('localhost', '8021', 'ClueCon')
r = redis.StrictRedis(host='23.253.221.228', port=6379, db=0)

if con.connected:
    con.events('plain', 'all')
    while 1:
        e = con.recvEvent()
        if e:
            #print e.serialize()
            if e.getHeader("Event-Name") == "CUSTOM":
                if e.getHeader("Event-Subclass") == "conference::maintenance":
                        if e.getHeader("Conference-Name") == "demo":
                                print e.getHeader("Action")
                                if e.getHeader("Action") == "conference-create":
                                        r.set('demo', e.getHeader("FreeSWITCH-IPv4"))
                                        r.set('demo_status', 1 )
                                        print("demo new conference created");

                                if e.getHeader("Action") == "conference-destroy":
                                        r.delete(['demo'])
                                        r.set('demo_status', 0 )
                                        print("demo conference destroy");
                                '''
                                print e.getHeader("Event-Name")
                                print e.getHeader("Event-Subclass")
                                print e.getHeader("Event-Calling-Function")
                                print e.getHeader("Conference-Name")
                                print e.getHeader("Action")
                                '''
