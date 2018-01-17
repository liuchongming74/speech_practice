from state import State


def transition_probability(prb, emission, last_state, current_state, trans_prb_table, emission_prb_table):
    """
    当前状态概率 = 上一次最优概率 * 状态转移概率 * 序列发射概率
    :param prb:                 上一次最优概率
    :param last_state:          上一次状态
    :param emission:            当前时刻发射概率
    :param current_state:       当前状态
    :param trans_prb_table:     状态转移概率表
    :param emission_prb_table:  状态发射字符概率
    :return:
    """
    if last_state is None:
        return emission_prb_table[current_state][emission]
    return prb * trans_prb_table[last_state][current_state] * emission_prb_table[current_state][emission]


def reverse(state_node):
    """
    回溯删除不可行路径，将next—state从父节点的next列表中移除
    :return:
    """
    previous_node = state_node.previous
    if previous_node is None:
        # 如果到起始点则返回
        return
    previous_node.next.remove(state_node)
    # 如果该状态没有子状态，则说明该状态可以从路径中移除，递归删除
    if len(previous_node.next) == 0:
        reverse(previous_node)


def search_state(last_state, order, transition_prb_table, emission_prb_table, days):
    """
    搜索HMM状态网格，由于可能会存在多条概率相同的路径，因此要用列表存储
    :param last_state:          上一次的状态列表
    :param order:               时间序列索引
    :param transition_prb_table:状态转移概率表
    :param emission_prb_table:  状态发射字符概率表
    :param days:                观测序列
    :return:
    """
    if order == len(days):
        return last_state
    humidity = days[order]
    # 记录当前路径中最大的概率，用于回溯修剪路径
    current_path_maximum_prb = -1
    # 从前向后搜索最佳的状态结点
    for state in last_state:
        maximum_prb = -1
        next_state = []
        for current_state in transition_prb_table.keys():
            # 判断转移到哪个状态输出目标天气的概率最大
            prb = transition_probability(state.prb, humidity, state.state, current_state, transition_prb_table,
                                         emission_prb_table)
            if prb > maximum_prb:
                maximum_prb = prb
                next_state.clear()
                next_state.append(State(current_state, maximum_prb, state))
            elif prb == maximum_prb:
                next_state.append(State(current_state, maximum_prb, state))
        state.next += next_state
        if maximum_prb > current_path_maximum_prb:
            current_path_maximum_prb = maximum_prb
    # 记录当前状态，用于下一次递归
    current_states = []
    # 保留概率高的子路径
    for state in last_state:
        for node in state.next:
            if node.prb < current_path_maximum_prb:
                state.next.remove(node)
            else:
                current_states.append(node)
        # 如果当前状态没有向外继续转移，则递归删除路径
        if len(state.next) == 0:
            reverse(state)
    # 递归寻找转移状态
    return search_state(current_states, order + 1, transition_prb_table, emission_prb_table, days)


def init_data():
    """
    初始化数据
    :return:
    """
    trans_prb_table = {'sunny': {'sunny': 0.5, 'cloudy': 0.375, 'rainy': 0.125},
                       'rainy': {'sunny': 0.25, 'cloudy': 0.375, 'rainy': 0.375},
                       'cloudy': {'sunny': 0.25, 'cloudy': 0.125, 'rainy': 0.625}}
    emission_prb_table = {'sunny': {'dry': 0.6, 'dryish': 0.2, 'damp': 0.15, 'soggy': 0.05},
                          'rainy': {'dry': 0.05, 'dryish': 0.1, 'damp': 0.35, 'soggy': 0.5},
                          'cloudy': {'dry': 0.25, 'dryish': 0.3, 'damp': 0.2, 'soggy': 0.25}}
    return trans_prb_table, emission_prb_table


def display_state_transfer_path(node):
    # print(node.state)
    stack = [node.state]
    for child in node.next:
        display_state_transfer_path(child)


def backwards(state_node):
    """
    采用栈模拟，递归输出状态序列
    :param state_node: 状态节点
    :return:
    """
    stack = [state_node.state]
    # 如果当前状态节点的前趋节点为起始节点，则递归返回
    if state_node.previous.prb == -1:
        return [state_node.state]
    stack += backwards(state_node.previous)
    return stack


if __name__ == '__main__':
    transition_prb_table, emission_prb_table = init_data()
    days = ['dry', 'damp', 'soggy']
    # 记录路径起点
    start_point = State('start', -1, None)
    # 初始化初始状态
    maximum = -1
    states = []
    for state in transition_prb_table.keys():
        prb = transition_probability(1, days[0], None, state, transition_prb_table, emission_prb_table)
        if prb > maximum:
            states.clear()
            states.append(State(state, prb, start_point))
            maximum = prb
        elif prb == maximum:
            states.append(State(state, prb, start_point))
    start_point.next += states
    end_states = search_state(states, 1, transition_prb_table, emission_prb_table, days)
    # 采用栈模拟，输出状态表
    for state in end_states:
        state_seq = backwards(state)
        # 状态出栈
        while len(state_seq) != 0:
            print(state_seq.pop())
