from mininet.topo import Topo

class Lab3Topo(Topo):
    def build(self):
        h1 = self.addHost('h1') # server
        h2 = self.addHost('h2')
        h3 = self.addHost('h3')
        h4 = self.addHost('h4')

        s1 = self.addSwitch('s1')
        s2 = self.addSwitch('s2')
        s3 = self.addSwitch('s3')

        self.addLink(h1,s1)
        self.addLink(h2,s2)
        self.addLink(h3,s3)
        self.addLink(h4,s1)
        self.addLink(s1,s2)
        self.addLink(s1,s3)

topos = {'lab3topo':(lambda:Lab3Topo())}
