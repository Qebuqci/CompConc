public class Leitor extends Thread 
{
  private int id; //identificador da thread
  
  private Monitor monitor;  //objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  public Leitor(int id, Monitor m) 
  {
    this.id = id;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run ()
  {
      double j=777777777.7;
      while (true) {
        this.monitor.EntraLeitor(this.id);
        if (Principal.variavel % 2 == 0)
          System.out.println("É par");
        else
          System.out.println("É ímpar");
        this.monitor.SaiLeitor(this.id);
        
        // Processamento bobo qualquer
          for (int i=0; i<1000000000; i++) 
            j = j / 2;  //...loop bobo para simbolizar o tempo de escrita
      }
  }

}
