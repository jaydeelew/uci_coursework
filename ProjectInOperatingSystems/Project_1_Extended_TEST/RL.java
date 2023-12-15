import java.util.LinkedList;

public class RL {
    private LinkedList<Integer> priority2;
    private LinkedList<Integer> priority1;
    private LinkedList<Integer> priority0;
    
    
    public RL() {
        priority2 = new LinkedList<Integer>();
        priority1 = new LinkedList<Integer>();
        priority0 = new LinkedList<Integer>();
    }
    
    public int front() {
        if (priority2.size() > 0) {
            return priority2.peekFirst();
        }
        else if (priority1.size() > 0) {
            return priority1.peekFirst();
        }
        else if (priority0.size() > 0) {
            return priority0.peekFirst();
        }
        else {
            return -1;
        }
    }

    public int frontpriority() {
        if (priority2.size() > 0) {
            return 2;
        }
        else if (priority1.size() > 0) {
            return 1;
        }
        else if (priority0.size() > 0) {
            return 0;
        }
        else {
            return -1;
        }
    }

    // public int pidpriority(int pid) {
    //     int rlindex = -1;
    //     rlindex = priority2.indexOf(Integer.valueOf(pid));
    //     if (rlindex != -1) {
    //         return 2;
    //     }
    //     rlindex = priority1.indexOf(Integer.valueOf(pid));
    //     if (rlindex != -1) {
    //         return 1;
    //     }
    //     rlindex = priority0.indexOf(Integer.valueOf(pid));
    //     if (rlindex != -1) {
    //         return 0;
    //     }
    //     return -1;
    // }

    public int remove(int pid) {
        int rlindex = -1;
        rlindex = priority2.indexOf(Integer.valueOf(pid));
        if (rlindex != -1) {
            priority2.remove(rlindex);
            return 1;
        }
        rlindex = priority1.indexOf(Integer.valueOf(pid));
        if (rlindex != -1) {
            priority1.remove(rlindex);
            return 1;
        }
        rlindex = priority0.indexOf(Integer.valueOf(pid));
        if (rlindex != -1) {
            priority0.remove(rlindex);
            return 1;
        }
        return -1;
    }

    public int dequeue() {
        if (priority2.size() > 0) {
            return priority2.removeFirst();
        }
        if (priority1.size() > 0) {
            return priority1.removeFirst();
        }
        if (priority0.size() > 0) {
            return priority0.removeFirst();
        }
        return -1;
    }

    public void enqueue(int priority, int pid) {
        switch (priority) {
            case 2:
                priority2.addLast(pid);
                break;
            case 1:
                priority1.addLast(pid);
                break;
            case 0:
                priority0.addLast(pid);
                break;
            default:
                System.out.println("* could not enqueue");
        }
    }

    // overidden toString()
    public String toString() {
        return "\nPriority 2: " + this.priority2 +
        "\nPriority 1: " + this.priority1 +
        "\nPriority 0: " + this.priority0; 
    }
}
