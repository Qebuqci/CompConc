public class Monitor 
{
 private int leit, escr;  
  
  // Construtor
  public Monitor() 
  { 
     this.leit = 0; //leitores lendo (0 ou mais)
     this.escr = 0; //escritor escrevendo (0 ou 1)
  } 
  
  // Entrada para leitores
  public synchronized void EntraLeitor(int id) 
  {
    try { 
      while (this.escr > 0) {
      //if (this.escr > 0) {
         System.out.println("le.leitorBloqueado(" + id + ")" );
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.leit++;  //registra que ha mais um leitor lendo
      System.out.println("le.leitorLendo(" + id + ")" );
    } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor(int id) 
  {
     this.leit--; //registra que um leitor saiu
     if (this.leit == 0) 
           this.notify(); //libera escritor (caso exista escritor bloqueado)
     System.out.println("le.leitorSaindo("+id+")");
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor(int id) 
  {
    try { 
      while ((this.leit > 0) || (this.escr > 0)) {
      //if ((this.leit > 0) || (this.escr > 0)) {
         System.out.println("le.escritorBloqueado(" + id + ")" );
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.escr++; //registra que ha um escritor escrevendo
      System.out.println ("le.escritorEscrevendo(" + id + ")" );
    } 
    catch (InterruptedException e) {}
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id) 
  {
     this.escr--; //registra que o escritor saiu
     notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
     System.out.println("le.escritorSaindo(" + id + ")" );
  }
}
