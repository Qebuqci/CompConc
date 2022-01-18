import java.lang.Thread;

class Soma_Conc implements Runnable
{
    int id_thread, nthreads, tam_vetor;
    int[] vetor;
    
    public static int soma = 0;  // Variável da classe para acumular a soma dos inteiros
    
    // Estratégia de Divisão em blocos
    int tam_bloco;
    // Posições de inicio e fim de cada thread no cálculo da soma dos inteiros do vetor
    int inicioDaThread; 
    int fimDaThread; 
    
    public Soma_Conc(int[] vetor, int tam_vetor, int nthreads, int id_thread)
    {
       this.vetor = vetor;
       this.tam_vetor = tam_vetor;
       this.nthreads = nthreads;
       this.id_thread = id_thread;
    }

    public void run()
    {
        tam_bloco = tam_vetor / nthreads;
        inicioDaThread = id_thread * tam_bloco;
        if (id_thread == nthreads - 1) {
            fimDaThread = tam_vetor;
            for (int i = inicioDaThread; i <= fimDaThread; i++)
                soma += vetor[i];
        }
        else {
            fimDaThread = inicioDaThread + tam_bloco;
            for (int i = inicioDaThread; i < fimDaThread; i++)
                soma += vetor[i];
        }
       
    }
    
    public static int getSoma()
    {
        return soma;
    }

}


public class Principal_Lab7
{

    public static void main(String[] args) 
    {
        int[] vetor;
        Thread[] threads;
        int tam_vetor, nthreads;
    
        // 1. Inicialização do programa
        if (args.length != 2) {
            System.out.println("Uso: java Lab7 <tamanho do vetor> <nº de threads>");
            return ;
        }

        tam_vetor = Integer.parseInt(args[0]);
        nthreads = Integer.parseInt(args[1]);
        
        if (tam_vetor < nthreads) 
            nthreads = tam_vetor;
        
        // 2. Inicialização do vetor
        vetor = new int[tam_vetor + 1];
        for (int i = 0; i <= tam_vetor; i++ )
            vetor[i] = 2*i; // Vetor contendo os n(tam_vetor) primeiros nº pares
        
        // 3. Criação das threads
        threads = new Thread[nthreads];
        for (int i = 0; i < nthreads; i++)
            threads[i] = new Thread(new Soma_Conc(vetor, tam_vetor, nthreads, i));
        
        // 4. Inicialialização das threads
        for (int i = 0; i < nthreads; i++)
            threads[i].start();
        
        // 5. Espera todas as threads terminarem
        for (int i=0; i<threads.length; i++) {
            try {
                threads[i].join(); } 
            catch (InterruptedException e) {
                return ; }
        }

        System.out.println("Valor de soma = " + Soma_Conc.getSoma()); 
    }
        
}
