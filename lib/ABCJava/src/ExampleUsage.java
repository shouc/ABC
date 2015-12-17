import java.math.BigDecimal;
import java.util.Map;
import java.util.Map.Entry;

import vlab.cs.ucsb.edu.DriverProxy;
import vlab.cs.ucsb.edu.DriverProxy.Option;

public class ExampleUsage {

  public static void main(String[] args) {
    // TODO Auto-generated method stub
    DriverProxy abcDriver = new DriverProxy();
    abcDriver.setOption(Option.OUTPUT_PATH, "abc");
    abcDriver.setOption(Option.MODEL_COUNTER_ENABLED, true);
    abcDriver.setOption(Option.LIA_ENGINE_ENABLED, true);
    
    String constraint = "(set-logic QF_S)\n"
        + "(declare-fun var_abc () String)\n"
        + "(assert (= var_abc (concat \"a\" \"b\")))\n"
        + "(check-sat)";
    
    boolean result = abcDriver.isSatisfiable(constraint);
    
    if (result) {
      System.out.println("Satisfiable");
      BigDecimal count = abcDriver.count("var_abc", 10);
      if (count != null) {
        System.out.println(count.toString());
      } else {
        System.out.println("An error occured during counting, please contact vlab@cs.ucsb.edu");
      }
      
      abcDriver.printResultAutomaton();
      
      Map<String, String> results = abcDriver.getSatisfyingExamples();
      for (Entry<String, String> var_result : results.entrySet()) {
        System.out.println(var_result.getKey() + " : \"" + var_result.getValue() + "\"");
      }
    } else {
      System.out.println("Unsatisfiable");
    }
    
    abcDriver.dispose(); // release resources
  }
}
