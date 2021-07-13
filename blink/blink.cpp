kernel ParticleExampleKernel : ImageComputationKernel<ePixelWise>
{
  // Declare the particle attributes as Images:
  Image<eReadWrite> p_velocity;

  // Declare our parameter storage
  param:
    float _amount;
    float _dt;

  // Define the parameters to the outside world
  void define() {
    defineParam(_amount, "paAmount", 0.0f);
    defineParam(_dt, "_dt", 1.0f);
  }

  // Process the particle attributes
  void process() {
    p_velocity() *= (1.0f-_amount*_dt);
  }
};
