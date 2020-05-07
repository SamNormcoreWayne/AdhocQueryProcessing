# Ad-hoc OLAP Query Processing

## 1. BackGround

1. MF Query: Damianos Chatziantoniou and Kenneth A. Ross. 1996. [**Querying Multiple Features of Groups in Relational Databases.**](https://dl.acm.org/doi/abs/10.5555/645922.673628) In Proceedings of the 22th International Conference on Very Large Data Bases (VLDB ’96). Morgan Kaufmann Publishers Inc., San Francisco, CA, USA, 295–306.
2. Ad-Hoc OLAP Query: D. Chatziantoniou, [**"Evaluation of ad hoc OLAP: in-place computation"**](https://ieeexplore.ieee.org/document/787619) ,Proceedings. Eleventh International Conference on Scientific and Statistical Database Management, Cleveland, OH, USA, 1999, pp. 34-43, doi: 10.1109/SSDM.1999.787619.

## 2. Simplify:

- Example EMF Query:

    ```SQL
    select cust, sum(x.quant), sum(y.quant), sum(z.quant)
        from sales
        group by cust: x, y, z
        such that x.state = ‘NY’
            and y.state = ‘NJ’
            and z.state = ‘CT’
        having
            sum(x.quant) > 2 * sum(y.quant) or avg(x.quant) > avg(z.quant);
    ```

- Simplified Input:

    ```text
    SELECT ATTRIBUTE(S):
    cust, 1_sum_quant, 2_sum_quant, 3_sum_quant
    NUMBER OF GROUPING VARIABLES(n):
    3
    GROUPING ATTRIBUTES(V):
    cust
    F-VECT([F]):
    1_sum_quant, 1_avg_quant, 2_sum_quant, 3_sum_quant, 3_avg_quant
    SELECT CONDITION-VECT([σ]):
    1.state=’NY’
    2.state=’NJ’
    3.state=’CT’
    HAVING_CONDITION(G):
    1_sum_quant > 2 * 2_sum_quant or 1_avg_quant > 3_avg_quant
    ```

## 3. System Design

1. Input Parser:

    - Files under `MFQueryInputParser/src/` :  
        - Implemented with C++11. Thanks to [@nlohmann](https://github.com/nlohmann) and his json parser [nlohann/json](https://github.com/nlohmann/json). Also, even though I did not write test cases(, which is bad!), I would give my thanks to [@google](https://github.com/google) and [google/googletest](https://github.com/google/googletest)  
        - This parser generates the codes in 3.2

2. DB Connector and Data Processing:

    - PostgerDB is used and it is dockerized. docker file is under ```/docker```

    - Implement ```Algorithm 3.1``` in paper 2. Codes under ```MFQueryInputParser/bin/*.py```. Implemented by Python 3.6

## 4. System Requirements

1. Make sure your Python version is 3.6.*

2. Your complier of C++ should at least support C++11(My Complier is g++ 7.4.0)

3. Make sure you are in Linux! (Windows/MacOS) not tested yet!
