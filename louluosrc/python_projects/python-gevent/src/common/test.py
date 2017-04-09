import singleton
class A(object):
    __metaclass__ = singleton.singleton

    def __init__(self):
        self.lol = 3

    def change(self, nb):
        self.lol += nb



if __name__ == '__main__':
    x = A()
    print x.lol
    m = A()
    print m.lol
    m.change(100)
    print m.lol
    n = A()
    print n.lol
