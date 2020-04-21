public class Min{
    public static void main(String args[]){
        int min = Integer.MAX_VALUE;
        for(int i = 0; i < args.length; i++){
            if(Integer.parseInt(args[i]) < min){
                min = Integer.parseInt(args[i]);
            }
        }
        System.out.println(min);
    }
}
