import java.util.LinkedList;

class ResourceEntry {
    public int r;
    public int amount;

    public ResourceEntry(int r, int amount) {
        this.r = r;
        this.amount = amount;
    }
}

public class PCB {
    public int priority; // set to 0, 1, or 2
    public int state; // ready-1 or blocked-0
    public int parent; // index of parent process
    public LinkedList<Integer> children; // indices of child processes
    public LinkedList<ResourceEntry> resources; // indices of resources and number of units amount
    
    public PCB(int parent, int priority) {
        this.priority = priority;
        state = 1;
        this.parent = parent;
        children = new LinkedList<Integer>();
        resources = new LinkedList<ResourceEntry>();
    }
    
    public void addresource(int r, int amount) {
        for (int i = 0; i < resources.size(); i++) {
            if (resources.get(i).r == r) {
                resources.get(i).amount += amount;
                return;
            }
        }
        ResourceEntry re = new ResourceEntry(r, amount);
        resources.add(re);
    }
    
    public int removeresource(int r) {
        int amount = 0;
        for (int i = 0; i < resources.size(); i++) {
            ResourceEntry re = resources.get(i); 
            if (re.r == r) {
                amount = re.amount;
                resources.remove(i);
                return amount;
            }
        }
        return amount;
    }

    public int resourceamountheld(int r) {
        int amount = 0;
        for (int i = 0; i < resources.size(); i++) {
            ResourceEntry re = resources.get(i);
            if (re.r == r) {
                amount += re.amount;
                break;
            }
        }
        return amount;
    }

    public void withdrawresource(int r, int k) {
        for (int i = 0; i < resources.size(); i++) {
            if (resources.get(i).r == r) {
                resources.get(i).amount -= k;
                if (resources.get(i).amount == 0) {
                    resources.remove(i);
                }
                break;
            }
        }
    }

    public String toString() { // overidden toString()
        return "\npriority: " + this.priority + 
                "\nstate: " + this.state + 
                "\nparent: " + this.parent +
                "\nchildren: " + this.children.toString() +
                "\nresources " + this.resources.toString();
    }
}
