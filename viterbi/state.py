class State():
    def __init__(self, state, prb, previous):
        # 当前状态的标识，如:sunny，rainy...
        self.state = state
        # 当前状态的选择概率
        self.prb = prb
        # 父状态
        self.previous = previous
        # 子状态
        self.next = []
