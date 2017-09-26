#define analogInPin PA5
#define maxSamples 5000

uint16_t dataPoints[maxSamples];

void fetch() {  
//  const adc_dev *dev = PIN_MAP[analogInPin].adc_device;
  int channel = PIN_MAP[analogInPin].adc_channel;
  adc_set_sample_rate(ADC1, ADC_SMPR_1_5); // ADC_SMPR_13_5, ADC_SMPR_1_5
  adc_set_sample_rate(ADC2, ADC_SMPR_1_5); // ADC_SMPR_13_5, ADC_SMPR_1_5
  adc_set_reg_seqlen(ADC1, 1);
  adc_set_reg_seqlen(ADC2, 1);
  ADC1->regs->SQR3 = channel;
  ADC2->regs->SQR3 = channel;

  ADC1->regs->CR1 |= 0x70000; // ADC_CR1_FASTINT;
  ADC1->regs->CR2 |= ADC_CR2_CONT | ADC_CR2_SWSTART;
  ADC2->regs->CR2 |= ADC_CR2_CONT | ADC_CR2_SWSTART;
  ADC2->regs->CR1 |= 0x70000; // ADC_CR1_FASTINT;
  nvic_globalirq_disable();
//  uint32_t t = micros();
  // .584 uS
  for (int j = 0; j < maxSamples ; j+=2 )
  {
    while (!(ADC1->regs->SR & ADC_SR_EOC))
        ;
    dataPoints[j] = ADC1->regs->DR & ADC_DR_DATA;
    while (!(ADC2->regs->SR & ADC_SR_EOC))
        ;
    dataPoints[j+1] = ADC2->regs->DR & ADC_DR_DATA;
  }
  nvic_globalirq_enable();
  //t = micros()-t;
  //Serial.println(String(t));
}


void setup() {
  Serial.begin(614400);
  pinMode(PA2,OUTPUT);
  digitalWrite(PA2,0);
  delay(3000);
}

uint32_t runNumber = 0;

void loop() {
  // put your main code here, to run repeatedly:
  fetch();
  for(int i=0;i<maxSamples;i++) {
      Serial.println(String(runNumber)+","+String(i)+","+String(dataPoints[i]));
  }  
  runNumber++;
}

