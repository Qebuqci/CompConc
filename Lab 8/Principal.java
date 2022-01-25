public class Principal
{
  // Número de threads : leitores, escritoras e leitoras/escritoras
  private static int E, L, LE;
  
  // Variavel compartilhada entre as threads
  public static int variavel;

  public static void main(String[] args)
  {
    if (args.length != 3) {
      System.out.println("Uso: java Principal <L> <E> <LE> \nL: nº threads Leitoras \nE: nº threads Escritoras \nLE: nº threads Leitoras/Escritoras ");
      return ;
    }
    
    E = Integer.valueOf(args[0]);
    L = Integer.valueOf(args[1]);
    LE = Integer.valueOf(args[2]);
    
    Monitor m = new Monitor();
    
    Principal.variavel = 0;
   
    Thread[] tescritoras = new Thread[E];
    Thread[] tleitoras = new Thread[L];
    Thread[] tleitescr = new Thread[LE];
     
    // Criando as threads Leitoras
    for (int i = 0; i < L; i++) {
      tleitoras[i] = new Leitor(i + 1, m);
      tleitoras[i].start();
    }
  
    // Criando as threads Escritoras
    for (int i = 0; i < E; i++) {
      tescritoras[i] = new Escritor(i + 1, m);
      tescritoras[i].start();
    }
    
    // Criando as threads Leitoras/Escritoras
    for (int i = 0; i < LE; i++) {
      tleitescr[i] = new LeitorEscritor(i + 1, m);
      tleitescr[i].start();
    }
        
  } 
  
}
