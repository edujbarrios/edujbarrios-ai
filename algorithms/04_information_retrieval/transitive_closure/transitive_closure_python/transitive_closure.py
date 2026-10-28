"""All-pairs reflexive reachability by Warshall's dynamic programming."""


def transitive_closure(adjacency):
    """Return a binary n-by-n reflexive reachability matrix."""
    if not isinstance(adjacency,(list,tuple)) or not 1<=len(adjacency)<=64:
        raise ValueError("square graph of size 1..64 required")
    n=len(adjacency)
    reach=[]
    for row in adjacency:
        if not isinstance(row,(list,tuple)) or len(row)!=n:
            raise ValueError("adjacency matrix must be square")
        checked=[]
        for x in row:
            if isinstance(x,bool) or not isinstance(x,(int,float)) or x not in (0,1):
                raise ValueError("binary adjacency values required")
            checked.append(int(x))
        reach.append(checked)
    for i in range(n):
        reach[i][i]=1
    for k in range(n):
        for i in range(n):
            if reach[i][k]:
                for j in range(n):
                    if reach[k][j]:
                        reach[i][j]=1
    return reach
