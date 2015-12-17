/*
 * vlab_cs_ucsb_edu_DriverProxy.cpp
 *
 *  Created on: Aug 26, 2015
 *      Author: baki
 */

#include <map>
#include <string>
#include <iostream>

#include "vlab_cs_ucsb_edu_DriverProxy.h"
#include "Driver.h"


jfieldID getHandleField(JNIEnv *env, jobject obj)
{
    jclass c = env->GetObjectClass(obj);
    // J is the type signature for long:
    return env->GetFieldID(c, "driverPointer", "J");
}

template <typename T>
T *getHandle(JNIEnv *env, jobject obj)
{
    jlong handle = env->GetLongField(obj, getHandleField(env, obj));
    return reinterpret_cast<T *>(handle);
}

template <typename T>
void setHandle(JNIEnv *env, jobject obj, T *t)
{
    jlong handle = reinterpret_cast<jlong>(t);
    env->SetLongField(obj, getHandleField(env, obj), handle);
}

/*
 * Class:     vlab_cs_ucsb_edu_DriverProxy
 * Method:    initABC
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_vlab_cs_ucsb_edu_DriverProxy_initABC (JNIEnv *env, jobject obj, jint log_flag) {

  Vlab::Driver *abc_driver = new Vlab::Driver();
  abc_driver->initializeABC((int)log_flag);
  setHandle(env, obj, abc_driver);
}

/*
 * Class:     vlab_cs_ucsb_edu_DriverProxy
 * Method:    setOption
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_vlab_cs_ucsb_edu_DriverProxy_setOption__IZ (JNIEnv *env, jobject obj, jint option, jboolean value) {
  Vlab::Driver *abc_driver = getHandle<Vlab::Driver>(env, obj);
  int opt = (int)option;
  abc_driver->setOption(static_cast<Vlab::Option::Name>(opt), (bool)value);
}

/*
 * Class:     vlab_cs_ucsb_edu_DriverProxy
 * Method:    setOption
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_vlab_cs_ucsb_edu_DriverProxy_setOption__ILjava_lang_String_2 (JNIEnv *env, jobject obj, jint option, jstring value) {
  Vlab::Driver *abc_driver = getHandle<Vlab::Driver>(env, obj);
  const char* string_value_str = env->GetStringUTFChars(value, JNI_FALSE);
  std::string string_value = string_value_str;
  int opt = (int)option;
  abc_driver->setOption(static_cast<Vlab::Option::Name>(opt), string_value);
  env->ReleaseStringUTFChars(value, string_value_str);
}

/*
 * Class:     vlab_cs_ucsb_edu_DriverProxy
 * Method:    isSatisfiable
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_vlab_cs_ucsb_edu_DriverProxy_isSatisfiable (JNIEnv *env, jobject obj, jstring constraint) {

  Vlab::Driver *abc_driver = getHandle<Vlab::Driver>(env, obj);
  std::istringstream input_constraint;
  const char* constraint_str = env->GetStringUTFChars(constraint, JNI_FALSE);
  input_constraint.str(constraint_str);
  abc_driver->reset();
  abc_driver->parse(&input_constraint);
  env->ReleaseStringUTFChars(constraint, constraint_str);
  abc_driver->initializeSolver();
  abc_driver->solve();
  bool result = abc_driver->isSatisfiable();
  return (jboolean)result;
}

/*
 * Class:     vlab_cs_ucsb_edu_DriverProxy
 * Method:    countVar
 * Signature: (Ljava/lang/String;IZ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_vlab_cs_ucsb_edu_DriverProxy_countVar (JNIEnv *env, jobject obj, jstring varName, jint bound, jboolean count_less_than_or_equal_bound) {
  Vlab::Driver *abc_driver = getHandle<Vlab::Driver>(env, obj);
  const char* var_name_str = env->GetStringUTFChars(varName, JNI_FALSE);
  std::string var_name = var_name_str;
  std::string result = abc_driver->count(var_name, (int)bound, (bool)count_less_than_or_equal_bound);
  jstring resultString = env->NewStringUTF(result.c_str());
  env->ReleaseStringUTFChars(varName, var_name_str);
  return resultString;
}

/*
 * Class:     vlab_cs_ucsb_edu_DriverProxy
 * Method:    printResultAutomaton
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_vlab_cs_ucsb_edu_DriverProxy_printResultAutomaton__ (JNIEnv *env, jobject obj) {
  Vlab::Driver *abc_driver = getHandle<Vlab::Driver>(env, obj);
  if (abc_driver->isSatisfiable()) {
    int index = 0;
    for(auto& variable_entry : abc_driver->getSatisfyingVariables()) {
      std::cout << variable_entry.first->getName() << " : \"" << variable_entry.second->getASatisfyingExample() << "\"" << std::endl;
      switch (variable_entry.second->getType()) {
        case Vlab::Solver::Value::Type::INT_AUTOMATON:
        case Vlab::Solver::Value::Type::STRING_AUTOMATON: {
          abc_driver->printResult(variable_entry.second, std::cout);
          break;
        }
        default:
          break;
      }
    }
    std::cout.flush();
  }
}

/*
 * Class:     vlab_cs_ucsb_edu_DriverProxy
 * Method:    printResultAutomaton
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_vlab_cs_ucsb_edu_DriverProxy_printResultAutomaton__Ljava_lang_String_2 (JNIEnv *env, jobject obj, jstring filePath) {
  Vlab::Driver *abc_driver = getHandle<Vlab::Driver>(env, obj);
  const char* file_path_str = env->GetStringUTFChars(filePath, JNI_FALSE);
  std::string file_path = file_path_str;

  if (abc_driver->isSatisfiable()) {
    int index = 0;
    for(auto& variable_entry : abc_driver->getSatisfyingVariables()) {
      std::cout << variable_entry.first->getName() << " : \"" << variable_entry.second->getASatisfyingExample() << "\"" << std::endl;
      switch (variable_entry.second->getType()) {
        case Vlab::Solver::Value::Type::INT_AUTOMATON:
        case Vlab::Solver::Value::Type::STRING_AUTOMATON: {
          std::stringstream ss;
          ss << file_path << "_" << index << ".dot";
          std::string out_file = ss.str();
          abc_driver->inspectResult(variable_entry.second, out_file);
          break;
        }
        default:
          break;
      }
    }
    std::cout.flush();
  }
  env->ReleaseStringUTFChars(filePath, file_path_str);
}

/*
 * Class:     vlab_cs_ucsb_edu_DriverProxy
 * Method:    getSatisfyingExamples
 * Signature: ()Ljava/util/Map;
 */
JNIEXPORT jobject JNICALL Java_vlab_cs_ucsb_edu_DriverProxy_getSatisfyingExamples (JNIEnv *env, jobject obj) {
  Vlab::Driver *abc_driver = getHandle<Vlab::Driver>(env, obj);
  jclass hashMapClass = env->FindClass("java/util/HashMap");
  jmethodID hashMapCtor = env->GetMethodID(hashMapClass, "<init>", "()V");
  jobject map = env->NewObject(hashMapClass, hashMapCtor);

  std::map<std::string, std::string> results = abc_driver->getSatisfyingExamples();

  jmethodID hasMapPut = env->GetMethodID(hashMapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

  for (auto var_entry : results) {
    jstring var_name = env->NewStringUTF(var_entry.first.c_str());
    jstring var_value = env->NewStringUTF(var_entry.second.c_str());
    env->CallObjectMethod(map, hasMapPut, var_name, var_value);
  }

  return map;
}

/*
 * Class:     vlab_cs_ucsb_edu_DriverProxy
 * Method:    reset
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_vlab_cs_ucsb_edu_DriverProxy_reset (JNIEnv *env, jobject obj) {
  Vlab::Driver *abc_driver = getHandle<Vlab::Driver>(env, obj);
  abc_driver->reset();
}

/*
 * Class:     vlab_cs_ucsb_edu_DriverProxy
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_vlab_cs_ucsb_edu_DriverProxy_dispose (JNIEnv *env, jobject obj) {
  Vlab::Driver *abc_driver = getHandle<Vlab::Driver>(env, obj);
  Vlab::Driver *tmp = abc_driver;
  abc_driver = nullptr;
  setHandle(env, obj, abc_driver);
  delete tmp;
}



