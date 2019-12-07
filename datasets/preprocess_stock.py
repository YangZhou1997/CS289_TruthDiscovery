import numpy as np
from collections import defaultdict

stock_name = ['aapl', 'adbe', 'adsk', 'goog', 'csco', 'amzn']
truth_file = ['./jul/stock-2011-07-01-nasdaq-com.txt', './jul/stock-2011-07-04-nasdaq-com.txt', './jul/stock-2011-07-05-nasdaq-com.txt']
obsv_file = ['./jul/stock-2011-07-01.txt', './jul/stock-2011-07-04.txt', './jul/stock-2011-07-05.txt']

raw_kv = defaultdict(lambda: list())

if __name__ == '__main__':
    obsv_cnt = 0
    stock_cnt = 0
    for file_name in obsv_file:
        file = open(file_name, "r")
        line = file.readline()
        while line:
            line = line.strip().split()
            if line[1] in stock_name:
                openprice = ''
                if line[0] in ['screamingmedia', 'cio-com', 'stocknod', 'renewable-energy-world', 'ycharts-com', 
                                'marketwatch', 'minyanville', 'bostonmerchant', 'boston-com', 'business-insider', 
                                'financial-content', 'finance-abc7chicago-com', 'finance-abc7-com', 'paidcontent', 
                                'chron', 'finviz', 'hpcwire']:
                    openprice = line[5]
                elif line[0] == 'optimum':
                    openprice = line[8]
                elif line[0] in ['stockpickr', 'predictwallstreet', 'zacks', 'stocktwits']:
                    openprice = str(float(line[3].rstrip('$').strip('$')) + float(line[4]))
                elif line[0] in ['cnn-money', ]:
                    openprice = line[4].strip('data')
                elif line[0] in ['foxbusiness', ]:
                    openprice = str(float(line[2].rstrip('$').strip('$')) + float(line[3]))
                elif line[0] in ['simple-stock-quotes', ]:
                    openprice = str(float(line[5].rstrip('$').strip('$')) + float(line[7]))
                elif line[0] in ['thecramerreport', ]:
                    openprice = line[3]
                else:
                    openprice = line[4]
                raw_kv[line[0]].append(float(openprice.rstrip('$').strip('$')))

            line = file.readline()
        file.close()

    for (k, v) in raw_kv.items():
        print(k, v)
    sample = raw_kv['wallstreetsurvivor']
    quantile = np.quantile(sample, 0.75)
    print(quantile)


    file = open("./stock.dat", 'w')
    for (k, v) in raw_kv.items():
        cnt = 0
        v_len = len(v)
        for val in v:
            if val > quantile:
                file.write("1")
            else:
                file.write("0")
            if cnt == v_len - 1:
                file.write("\n")
            else:
                file.write(" ")
            cnt += 1
    file.close()
        
    # generate truth
    raw_truth = []
    for file_name in truth_file:
        file = open(file_name, "r")
        line = file.readline()
        while line:
            line = line.strip().split()
            if line[0] in stock_name:
                raw_truth.append(float(line[5].rstrip('$').strip('$')))
            line = file.readline()
        file.close()

    print(raw_truth)

    v_len = len(raw_truth)
    file = open("./stock_truth.dat", 'w')
    for val in raw_truth:
        if val > quantile:
            file.write("1")
        else:
            file.write("0")
        if cnt == v_len - 1:
            file.write("\n")
        else:
            file.write(" ")
        cnt += 1
    file.close()
        