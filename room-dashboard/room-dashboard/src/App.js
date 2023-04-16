import {CBadge, CButton, CCard, CCardBody, CCardFooter, CCardHeader, CCol, CContainer, CFormRange, CFormSwitch,CRow} from '@coreui/react'
import '@coreui/coreui/dist/css/coreui.min.css'
import './App.css'
import { useEffect, useState } from 'react'
import { CChart } from '@coreui/react-chartjs'
import axios from 'axios'

const App = () => {
  const [manageLight, setManageLight] = useState(true)
  const [manageRollerBlind, setManageRollerBlind] = useState(50)
  const [currentLightState, setCurrentLightState] = useState(null)
  const [currentRollerBlindState, setCurrentRollerBlindState] = useState(null)

  const [lightData, setLightData] = useState({
	label : [],
	data : []
  })

  const [rollerBlindData, setRollerBlindData] = useState({
	label : [],
	data : []
  })

  useEffect(() =>{
	if(lightData.data.length === 0 && lightData.label.length === 0 && rollerBlindData.data.length === 0 && rollerBlindData.label.length === 0){
		const interval = setInterval(() => requestChartData(), 5000)
		return () => clearInterval(interval)
	}
  }, [lightData,rollerBlindData])

  const requestChartData = () => {
	console.log('fatto')
	/*console.log(point.data)
	console.log(point.label)
	console.log(point)
	let new_data = point.data.slice()
	let new_label = point.label.slice()
	new_data.push('ON')
	new_label.push((point.label.length + 1).toString())
	console.log(new_data)
	console.log(new_label)
	let new_point = {
		label : new_label,
		data : new_data,
	}
	console.log(new_point)
	setPoint(new_point)
	console.log(point)*/
	axios.post('URL', {}, {
		headers: {
		  'Content-Type': 'multipart/form-data',
		  'Accept': 'application/json',
		},
	  }).then((response) => {
		console.log(response.data)
		/*if (response.data.success) {
		  console.log(response.data.payload)
		}
		else {
		  console.log(response.data.error.message)
		}*/
	})
  }

  const changeStatus = () => {
	axios.post('URL', { light: manageLight, rollerBlind : manageRollerBlind }, {
		headers: {
		  'Content-Type': 'multipart/form-data',
		  'Accept': 'application/json',
		},
	  }).then((response) => {
		console.log(response.data)
		/*if (response.data.success) {
		  console.log(response.data.payload)
		}
		else {
		  console.log(response.data.error.message)
		}*/
	})
  }

  return ( <>
		<CContainer fluid>
			<CRow>
				<h1 className='heading'>Smart Room Dashboard</h1>
			</CRow>
			<CRow>
				<CCol>
					<CCard className='current-status'>
						<CCardHeader>
							<h2>Current Room Status</h2>
						</CCardHeader>
						<CCardBody>
							<p><b>Light Status:</b> <CBadge color={currentLightState === null ? 'warning' : currentLightState ? 'success' : 'danger'}>{currentLightState === null ? 'NULL' : currentLightState ? 'ON' : 'OFF'}</CBadge></p>
							<p><b>Roller Blind Status:</b> <CBadge color={currentRollerBlindState === null ? 'warning' : 'info'}>{currentRollerBlindState === null ? 'NULL' : currentRollerBlindState}</CBadge></p>
						</CCardBody>
					</CCard>
				</CCol>
				<CCol>
					<CCard className='manage-status'>
						<CCardHeader><h2>Manage Room Status</h2></CCardHeader>
						<CCardBody>
							<CFormRange
        						label={'Roller Blind Level\t' + manageRollerBlind}
        						min={0}
        						max={100}
        						step={1}
								value={manageRollerBlind}
								onChange={(event) => setManageRollerBlind(event.target.value)}
    						></CFormRange>
							<CFormSwitch
								size='lg'
								label='Light'
								defaultChecked
								onClick={(event) => setManageLight(event.target.checked)}
      						></CFormSwitch>
						</CCardBody>
						<CCardFooter>
							<CButton
								color='primary'
								shape='rounded'
								type='button'
								onClick={() => changeStatus()}
							>Save
							</CButton>
						</CCardFooter>
					</CCard>
				</CCol>
			</CRow>
			<CRow>
				<CCol>
					<CCard className='chart-light'>
						<CCardHeader>
							<h2>Light Status Chart</h2>
						</CCardHeader>
						<CCardBody>
							<CChart 
								type='line'
								options={{
									responsive : true,
									plugins: {
										title : {
											display : true,
											text : 'Light Status'
										}			
									},
									scales : {
										y : {
											type : 'category',
											labels : ['ON', 'OFF'],
											offset : true,
										}
									},
									animation : false,
									elements: {
										line: {
										  borderWidth: 2,
										  tension: 0,
										},
										point: {
										  radius: 4,
										  hitRadius: 10,
										  hoverRadius: 4,
										},
									},
								}}
								customTooltips={false}
								data={{
									labels : lightData.label,
									datasets : [
										{
											label: 'Status',
											data: lightData.data,
											backgroundColor: '#E46651',
                  							borderColor: '#E46651',
                  							pointHoverBackgroundColor: '#E46651',
      										stepped: true,
										}
									]
								}}
							></CChart>
						</CCardBody>
					</CCard>
				</CCol>
			</CRow>
			<CRow>
				<CCol>
					<CCard className='chart-roller'>
						<CCardHeader>
							<h2>Roller Blind Status Chart</h2>
						</CCardHeader>
						<CCardBody>
							<CChart 
								type='line'
								options={{
									responsive : true,
									plugins: {
										title : {
											display : true,
											text : 'Roller Blind Status'
										}			
									},
									scales : {
										y : {
											offset : true,
										}
									},
									animation : false,
									elements: {
										line: {
										  borderWidth: 2,
										  tension: 0,
										},
										point: {
										  radius: 4,
										  hitRadius: 10,
										  hoverRadius: 4,
										},
									},
								}}
								customTooltips={false}
								data={{
									labels : rollerBlindData.label,
									datasets : [
										{
											label: 'Status',
											data: rollerBlindData.data,
											backgroundColor: '#E46651',
                  							borderColor: '#E46651',
                  							pointHoverBackgroundColor: '#E46651',
										}
									]
								}}
							></CChart>
						</CCardBody>
					</CCard>
				</CCol>
			</CRow>
		</CContainer>
		</>
  );
}

export default App;
