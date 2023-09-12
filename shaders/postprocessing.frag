uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main(){
	color = vec4(0.0f);
	vec3 sample[9];
	//sample from texture offsets if using convultion matrix
	if(chaos || shake)
		for(int i = 0; i < 9; i ++)
		sample[i] = vec3(texture(scene, TexCoords.st + offsets[i]));

	if(chaos) // process effects
	{
		for(int i = 0; i < 9; i++)
			color += vec4(sample[i] * edge_kernel[i], 0.0f);
		color.a = 1.0f;
	}
	else if (confuse)
	{
		color = vec4(1.0 - texture(scene, TexCoords).rgb, 1.0);
	}
	else if(shake)
	{
		for(int i = 0; i < 9; i++)
			color += vec4(sample[i] * blur_kernel[i], 0.0f);
		color.a = 1.0f;
	}
	else 
	{
		color = texture(scene, TexCoords);
	}
}