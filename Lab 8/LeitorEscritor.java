public class LeitorEscritor extends Thread
{

  private int id;
  private Monitor monitor;
  
  public LeitorEscritor(int id, Monitor m)
  {
    this.id = id;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run ()
  {
      double j=777777777.7;

      while (true) {
        // Lendo a variavel e imprimindo na tela
        this.monitor.EntraLeitor(this.id);
        System.out.println(Principal.variavel);
        this.monitor.SaiLeitor(this.id);
        
        // Processamento bobo qualquer
        for (int i=0; i<1000000000; i++) 
          j = j / 2;  //...loop bobo para simbolizar o tempo de escrita

        this.monitor.EntraEscritor(this.id);
        Principal.variavel += 1;
        this.monitor.SaiEscritor(this.id);
    }
  
  }
  
}
