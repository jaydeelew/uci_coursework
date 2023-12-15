import java.util.StringTokenizer;

public class VMManager {
    private static final int PMSZ = 524288;
    private static final int FRAMESZ = 512;
    private static final int BLOCKSZ = 512;

    private int[] PM = new int[PMSZ];
    // shouldn't if be 262144 rows? 2^18 rows x 2^9 columns = 2^27 (27 bits in VA)
    private int[][] Disk = new int[1024][512];

    public void initST(StringTokenizer st) {
        while (st.hasMoreTokens()) {
            int segment = Integer.parseInt(st.nextToken());
            int segsize = Integer.parseInt(st.nextToken());
            int frame = Integer.parseInt(st.nextToken());

            PM[2*segment] = segsize;
            // element contains start of pagetable (frame-number or disk-block-number) of segment
            // disk-block will be a negative integer
            PM[2*segment + 1] = frame;
        }
    } 

    public void initPTs(StringTokenizer st) {
        while (st.hasMoreTokens()) {
            int segment = Integer.parseInt(st.nextToken());
            int page = Integer.parseInt(st.nextToken());
            int frame = Integer.parseInt(st.nextToken());

            // if page-table resides on Disk
            if (PM[2*segment + 1] < 0) {
                int blocknum = Math.abs(PM[2*segment + 1]);
                // the page of segment's pagetable is located in this frame/block 
                Disk[blocknum][page] = frame;
            }
            // else page-table in Physical Memory 
            else {
                /*
                pm[2*segment + 1]                            -element contains frame-number of segment
                pm[2*segment + 1]*512                        -frame-number * size of a frame = index of start of frame holding pagetable
                pm[2*segment + 1]*512 + page                 -page is the offset into the pagetable to start of page
                pm[(pm[2*segment + 1])*512 + page] = frame   -the page of segment's pagetable is located in this frame/block
                */ 
                PM[(PM[2*segment + 1])*512 + page] = frame;
            }
        }
    }

    // read a diskBlock from Disk, copy the block to array beginning at arrayPosition
    private void read_block(int diskBlock, int[] array, int arrayPosition) {
        for (int i = arrayPosition, j = 0; i < arrayPosition + BLOCKSZ; i++, j++) {
            array[i] = Disk[diskBlock][j];
        }
    }

    private int firstfreeframe() {
        int[] allocatedFrames = new int[1024];
        allocatedFrames[0] = 1; // for ST
        allocatedFrames[1] = 1; // for ST
        // find all PTs and pages in memory
        // for the 512 segments in ST
        for (int i = 0; i < FRAMESZ; i++) {
            // frame/block num of each PT
            int ptFrameNum = PM[i*2 + 1];
            // pagetable location in PM[]
            int ptMemoryIdx = ptFrameNum * FRAMESZ;
            // if pagetable is in memory, allocate frame
            if (ptFrameNum > 0) {
                allocatedFrames[ptFrameNum] = 1; 
                // for each page in PT
                for (int j = ptMemoryIdx; j < ptMemoryIdx + FRAMESZ; j++) {
                    int pageFrameNum = PM[j];
                    // if page is in memory, allocate frame
                    if (pageFrameNum > 0) {
                        allocatedFrames[pageFrameNum] = 1; 
                    }
                }
            }
            // if pagetable is not im memory, do not allocate frame
            else if (ptFrameNum < 0) {
                int ptBlockNum = Math.abs(ptFrameNum);
                int[] blockArray = new int[512];
                read_block(ptBlockNum, blockArray, 0);
                for (int k = 0; k < BLOCKSZ; k++) {
                    int pageFrameNum = blockArray[k];
                    // if page is in memory, allocate frame
                    if (pageFrameNum > 0) {
                        allocatedFrames[pageFrameNum] = 1; 
                    }
                }
            }
        }
        // return first free frame in PM[]
        for (int i = 0; i < 1024; i++) {
            if (allocatedFrames[i] == 0) {
                return i;
            }
        }
        return -1;
    }

    public int translateaddress(String address) {
        int va;
        int pa;
        try {
            va = Integer.parseInt(address);
        } catch (NumberFormatException e) {
            return -1;
        }
        // segment number
        int s = va >> 18;
        // if segment number is not within the ST
        if (s < 0 || s > 511) {
            return -1;
        }
        // page of pagetable (step 1)
        int p = va >> 9;
        // page of pagetable (step 2)
        p = p & 0x1FF;
        // word offset in page
        int w = va & 0x1FF;
        // address offset in segment
        int pw = va & 0x3FFFF;
        // if va is beyond segment end
        if (pw >= PM[2*s]) {
            return -1;
        }
        // pagetable memory-frame number
        int ptFrameNum = PM[2*s + 1];
        
        // if pagetable is in memory
        if (ptFrameNum > 0) {
            // pagetable location in PM[]
            int ptMemoryIdx = ptFrameNum * FRAMESZ;
            // if page is on Disk[][]
            if (PM[ptMemoryIdx + p] < 0) {
                // page now in memory               
                PM[ptMemoryIdx + p] = firstfreeframe();
            }
        }
        // if pagetable is on Disk[][]
        else if (ptFrameNum < 0) {
            // pagetable disk-block number
            int ptBlockNum = Math.abs(ptFrameNum);
            int freeframe = firstfreeframe();
            // update pagetable frame in ST
            PM[2*s + 1] = freeframe;
            // new location of pagetable 
            int ptMemoryIdx = freeframe * FRAMESZ;

            read_block(ptBlockNum, PM, ptMemoryIdx);

            // if page is on Disk[][]
            if (PM[ptMemoryIdx + p] < 0) {
                // page now in memory               
                PM[ptMemoryIdx + p] = firstfreeframe();
            }
        }
        pa = PM[PM[2*s + 1]*512 + p]*512 + w;
        return pa;
    }
}